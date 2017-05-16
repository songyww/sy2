#include "process_with_surf.h"

using namespace std;
using namespace cv;


void findPairs3( const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
	const CvSeq* imageKeypoints, const CvSeq* imageDescriptors, vector<int>& ptpairs )
{
	int i;
	CvSeqReader reader, kreader;
	cvStartReadSeq(objectKeypoints, &kreader );
	cvStartReadSeq(objectDescriptors, &reader );
	ptpairs.clear();

	for ( i = 0; i < objectDescriptors->total; i ++ )
	{
		const CvSURFPoint* kp = ( const CvSURFPoint* )kreader.ptr;
		const float* descriptor = ( const float* )reader.ptr;
		CV_NEXT_SEQ_ELEM( kreader.seq->elem_size, kreader );
		CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );
		int nearest_neighbor = naiveNearestNeighbor3( descriptor, kp->laplacian, imageKeypoints, imageDescriptors );
		if ( nearest_neighbor >= 0 )
		{
			ptpairs.push_back( i );
			ptpairs.push_back( nearest_neighbor );
		}

	}
}

int naiveNearestNeighbor3( const float* vec, int laplacian,
	const CvSeq* model_keypoints,
	const CvSeq* modle_descriptors )
{
	int length = (int)( modle_descriptors->elem_size/sizeof(float) );
	int i, neighbor = -1;
	double d, dist1 = 1e6, dist2 = 1e6;
	CvSeqReader reader, kreader;
	cvStartReadSeq( model_keypoints, &kreader,  0 );
	cvStartReadSeq( modle_descriptors, &reader,  0 );

	for (i = 0; i < model_keypoints->total; i ++ )
	{
		const CvSURFPoint* kp = ( const CvSURFPoint* )kreader.ptr;
		const float* mvec = ( const float* )reader.ptr;
		CV_NEXT_SEQ_ELEM( kreader.seq->elem_size, kreader );
		CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );
		if ( laplacian != kp->laplacian )
			continue;
		d = compareSURFDescriptors3( vec, mvec, dist2, length );
		if ( d < dist1 )
		{
			dist2 = dist1;
			dist1 = d;
			neighbor = i;
		}
		else if ( d < dist2 )
			dist2 = d;
	}
	if ( dist1 < 0.55*dist2)
		return neighbor;
	return -1;
}

double compareSURFDescriptors3( const float* d1, const float* d2, double best, int length)
{
	double total_cost = 0;
	assert( length % 4 == 0 );
	for ( int i = 0; i < length; i += 4 )
	{
		double t0 = d1[i] - d2[i];
		double t1 = d1[i + 1] - d2[i + 1];
		double t2 = d1[i + 2] - d2[i + 2];
		double t3 = d1[i + 3] - d2[i + 3];
		total_cost += t0*t0 + t1*t1 +t2*t2 + t3*t3;
		if ( total_cost > best)
			break;
	}
	return total_cost;
}


CvMat* GetH3(vector<int>& ptpairs,CvSeq *CurrentKeypoints,CvSeq *PreKeypoints)
{
	CvMat* H;
	H = ransac_xform3( ptpairs,CurrentKeypoints,PreKeypoints, FEATURE_FWD_MATCH, 4, 0.02, 3.0 );
	return H;
}

CvMat* ransac_xform3( vector<int>& ptpairs,CvSeq *CurrentKeypoints,CvSeq *PreKeypoints, int mtype, 
	int m, double p_badxform, double err_tol)
{
	CvPoint2D64f* pts, * mpts;
	CvMat* M = NULL;
	CvMat* Max_M= NULL;

	double p, in_frac = 0.25;
	int i, nm, in, in_min, in_max = 0, k = 0;//n是number of features in feat

	nm = (int)ptpairs.size()/2;
	if( nm < m )
	{
		fprintf( stderr, "Warning: not enough matches to compute xform, %s" \
			" line %d\n", __FILE__, __LINE__ );
		return NULL;
	}

	in_min = calc_min_inliers3( nm, m, RANSAC_PROB_BAD_SUPP, p_badxform );

	p = pow( 1.0 - pow( in_frac, m ), k );
	i = 0;
	vector<int> Temp_ptpairs;
	vector<int> inliers_ptpairs;
	vector<int> sample_ptpairs;
	while( p > p_badxform )
	{
		draw_ransac_sample3( ptpairs,sample_ptpairs);
		extract_corresp_pts3( sample_ptpairs,CurrentKeypoints,PreKeypoints, m, mtype, &pts, &mpts );
		M = lsq_homog3(pts,mpts, m );

		if( ! M )
			goto iteration_end;
		in = find_consensus3( ptpairs,CurrentKeypoints,PreKeypoints, mtype, M, err_tol,Temp_ptpairs);
		if( in > in_max )
		{
			in_max = in;
			in_frac = (double)in_max / nm;
			inliers_ptpairs=Temp_ptpairs;

			Max_M=cvCloneMat(M);
		}
		cvReleaseMat( &M );

iteration_end:
		release_mem3( pts, mpts);
		p = pow( 1.0 - pow( in_frac, m ), ++k );
	}

	if( in_max >= in_min )
	{
		extract_corresp_pts3( inliers_ptpairs,CurrentKeypoints,PreKeypoints, in_max, mtype, &pts, &mpts );
		M = lsq_homog3( pts, mpts, in_max );

		in = find_consensus3(ptpairs,CurrentKeypoints,PreKeypoints, mtype, M, err_tol, inliers_ptpairs);
		cvReleaseMat( &M );
		release_mem3( pts, mpts );
		extract_corresp_pts3( inliers_ptpairs,CurrentKeypoints,PreKeypoints, in, mtype, &pts, &mpts );
		ptpairs.clear();
		ptpairs=inliers_ptpairs;//把内点序列存储在ptpairs内

		M = lsq_homog3( pts, mpts, in );
		release_mem3( pts, mpts );
	}

	Temp_ptpairs.clear();
	inliers_ptpairs.clear();
	sample_ptpairs.clear();
	return M;
}

static int calc_min_inliers3( int n, int m, double p_badsupp, double p_badxform )
{
	double pi, sum;
	int i, j;

	for( j = m+1; j <= n; j++ )
	{
		sum = 0;
		for( i = j; i <= n; i++ )
		{
			pi = pow( p_badsupp, i - m ) * pow( 1.0 - p_badsupp, n - i + m ) *
				gsl_sf_choose3( n - m, i - m );
			sum += pi;
		}
		if( sum < p_badxform )
			break;
	}
	return j;
}

static	void draw_ransac_sample3( vector<int> ptpairs,vector<int>& sample_ptpairs)
{
	int rank[4];
	for(int i=0;i<4;i++)
		rank[i]=-1;
	int j=0;
	int flag;

	sample_ptpairs.clear();
	while(j<4)
	{
		flag=0;
		int k=rand()%(int)ptpairs.size();
		k=k%2 ? k+1:k;
		for(int i=0;i<4;i++)
		{
			if(k==rank[i])
			{
				flag=1;
				break;
			}
		}
		if(flag==0 && k!=((int)ptpairs.size()))
		{
			rank[j]=k;
			j++;
		}
	}
	for(int i=0;i<4;i++)
	{
		int a=rank[i];
		sample_ptpairs.push_back(ptpairs[a]);
		sample_ptpairs.push_back(ptpairs[a+1]);
	}
}

static void extract_corresp_pts3( vector<int> ptpairs,CvSeq *CurrentKeypoints,CvSeq *PreKeypoints,int m, int mtype,
	CvPoint2D64f** pts, CvPoint2D64f** mpts )
{
	CvPoint2D64f* _pts, * _mpts;
	int i,j=0;
	_pts = (struct CvPoint2D64f *)calloc( m, sizeof( CvPoint2D64f ) );
	_mpts = (struct CvPoint2D64f *)calloc( m, sizeof( CvPoint2D64f ) );

	if( mtype == FEATURE_FWD_MATCH )
	{
		for(i=0;i<2*m;i+=2)
		{
			CvSURFPoint* r1 = (CvSURFPoint*)cvGetSeqElem(CurrentKeypoints, ptpairs[i] );
			_pts[j].x=(r1->pt.x);
			_pts[j].y=(r1->pt.y);
			CvSURFPoint* r2 = (CvSURFPoint*)cvGetSeqElem(PreKeypoints, ptpairs[i+1] );
			_mpts[j].x =(r2->pt.x);
			_mpts[j++].y =(r2->pt.y);
		}
	}
	*pts = _pts;
	*mpts = _mpts;
}

static int find_consensus3( vector<int> ptpairs,CvSeq *CurrentKeypoints,CvSeq *PreKeypoints, int mtype,
	CvMat* M, double err_tol,vector<int>& inliers_ptpairs)
{
	CvPoint2D64f pt, mpt;
	double err;
	int i, in = 0;
	double err_square=err_tol*err_tol;
	inliers_ptpairs.clear();
	if( mtype == FEATURE_FWD_MATCH )
		for( i = 0; i < (int)ptpairs.size(); i += 2 )
		{
			CvSURFPoint* r1 = (CvSURFPoint*)cvGetSeqElem( CurrentKeypoints, ptpairs[i] );
			CvSURFPoint* r2 = (CvSURFPoint*)cvGetSeqElem( PreKeypoints, ptpairs[i+1] );
			pt.x=r1->pt.x;
			pt.y=r1->pt.y;
			mpt.x=r2->pt.x;
			mpt.y=r2->pt.y;
			err = homog_xfer_err3( pt, mpt, M );
			if(err<=err_square)
			{
				inliers_ptpairs.push_back(ptpairs[i]);
				inliers_ptpairs.push_back(ptpairs[i+1]);
				in++;
			}
		}
		return in;
}



static	double homog_xfer_err3( CvPoint2D64f pt, CvPoint2D64f mpt, CvMat* H )
{
	CvPoint2D64f xpt = persp_xform_pt4( pt, H );
	double x_diff = xpt.x - mpt.x;
	double y_diff = xpt.y - mpt.y;

	return x_diff * x_diff + y_diff * y_diff;
}

static	CvPoint2D64f persp_xform_pt4( CvPoint2D64f pt, CvMat* T )
{
	CvMat XY, UV;
	double xy[3] = { pt.x, pt.y, 1.0 }, uv[3] = { 0 };
	CvPoint2D64f rslt;

	cvInitMatHeader( &XY, 3, 1, CV_64FC1, xy, CV_AUTOSTEP );
	cvInitMatHeader( &UV, 3, 1, CV_64FC1, uv, CV_AUTOSTEP );
	cvMatMul( T, &XY, &UV );
	rslt = cvPoint2D64f( uv[0] / uv[2], uv[1] / uv[2] );

	return rslt;
}


static CvMat* lsq_homog3( CvPoint2D64f* pts, CvPoint2D64f* mpts, int n )
{
	CvMat* H, * A, * B, X;
	double x[9];
	int i;

	if( n < 4 )
	{
		fprintf( stderr, "Warning: too few points in lsq_homog(), %s line %d\n",
			__FILE__, __LINE__ );
		return NULL;
	}


	A = cvCreateMat( 2*n, 8, CV_64FC1 );
	B = cvCreateMat( 2*n, 1, CV_64FC1 );
	X = cvMat( 8, 1, CV_64FC1, x );
	H = cvCreateMat(3, 3, CV_64FC1);
	cvZero( A );
	for( i = 0; i < n; i++ )
	{
		cvmSet( A, i, 0, pts[i].x );
		cvmSet( A, i+n, 3, pts[i].x );
		cvmSet( A, i, 1, pts[i].y );
		cvmSet( A, i+n, 4, pts[i].y );
		cvmSet( A, i, 2, 1.0 );
		cvmSet( A, i+n, 5, 1.0 );
		cvmSet( A, i, 6, -pts[i].x * mpts[i].x );
		cvmSet( A, i, 7, -pts[i].y * mpts[i].x );
		cvmSet( A, i+n, 6, -pts[i].x * mpts[i].y );
		cvmSet( A, i+n, 7, -pts[i].y * mpts[i].y );
		cvmSet( B, i, 0, mpts[i].x );
		cvmSet( B, i+n, 0, mpts[i].y );
	}
	cvSolve( A, B, &X, CV_SVD );
	x[8] = 1.0;
	X = cvMat( 3, 3, CV_64FC1, x );
	cvConvert( &X, H );

	cvReleaseMat( &A );
	cvReleaseMat( &B );
	return H;
}


double gsl_sf_choose3(unsigned int n, unsigned int m)
{
	unsigned long n1,m1,mfei;
	n1=m1=mfei=1;

	if(n=0)
		n1=1;

	if(m=0)
		return 0;
	else if(m==1)
		return n;
	else
	{
		int mfei=n-m;
		if(m<mfei)
		{
			for(int p=n;p>mfei;p--)
				n1*=p;
			for(int p=m;p>0;p--)
				m1*=p;
		}
		else
		{
			for(int p=n;p>m;p--)
				n1*=p;
			for(int p=mfei;p>0;p--)
				m1*=p;
		}
	}

	double k=n1/m1;
	return k;
}

static __inline void release_mem3( CvPoint2D64f* pts1, CvPoint2D64f* pts2)
{
	free( pts1 );
	free( pts2 );
}

IplImage* convert_to_gray8( IplImage* img )
{
	IplImage* gray8;

	gray8 = cvCreateImage( cvGetSize(img), IPL_DEPTH_8U, 1 );
	cvCvtColor( img, gray8, CV_RGB2GRAY );

	return gray8;
}


const char * cst_to_conchar( CString str )
{
	const size_t strsize = ( str.GetLength() + 1)*2;
	char * pstr = new char[strsize];
	size_t sz = 0;
	wcstombs_s( &sz, pstr, strsize, str, _TRUNCATE );
	int n = atoi(( const char*)pstr); 
	return pstr;
}

double GetRMSE3(vector<int> ptpairs,CvSeq* CurrentKeypoints,CvSeq* PreKeypoints,CvMat* H)
{
	double rmse=0.0;
	int m=ptpairs.size()/2;
	CvSURFPoint r1;
	CvSURFPoint r2;

	for(int i=0;i<m;i++)
	{
		r1.pt=((CvSURFPoint*)cvGetSeqElem( CurrentKeypoints, ptpairs[i*2] ))->pt;
		r2.pt=((CvSURFPoint*)cvGetSeqElem( PreKeypoints, ptpairs[i*2+1] ))->pt;
		r1.pt=persp_xform_pt3(r1.pt,H);
		rmse+=sqrt((r1.pt.x-r2.pt.x)*(r1.pt.x-r2.pt.x)+(r1.pt.y-r2.pt.y)*(r1.pt.y-r2.pt.y));

	}
	rmse=rmse/m;
	ptpairs.clear();
	return rmse;
}


CvPoint2D32f persp_xform_pt3( CvPoint2D32f pt, CvMat* T )
{
	CvMat XY, UV;
	double xy[3] = { pt.x, pt.y, 1.0 }, uv[3] = { 0 };
	CvPoint2D32f rslt;

	cvInitMatHeader( &XY, 3, 1, CV_64FC1, xy, CV_AUTOSTEP );
	cvInitMatHeader( &UV, 3, 1, CV_64FC1, uv, CV_AUTOSTEP );
	cvMatMul( T, &XY, &UV );
	rslt = cvPoint2D32f( uv[0] / uv[2], uv[1] / uv[2] );

	return rslt;
}
