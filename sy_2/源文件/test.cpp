H = H_2[0];

double smallhx,smallhy,ha,hb;///�ò�����ʾ�����ü�������Сͼ��ƽ�Ʋ���
double bighx,bighy;//�ò�����ʾ����׼ͼ������ڲο�ͼ���ƽ�Ʋ���
//px = cvmGet( H, 0, 2 );
//py = cvmGet( H, 1, 2 );
smallhx = cvmGet( H, 0, 2 ); //����������Ǵ���׼ͼ���и���Ȥ��������ڲο�ͼ���ƽ�ƾ���
smallhy = cvmGet( H, 1, 2 );
ha = cvmGet(H,2,0);
hb = cvmGet(H,2,1);
//���ڻ���Ҫ֪�� �ο�ͼ���е�Сͼ����ڴ�ͼ��λ�û�������
//���ο�ͼ���вü�����Сͼ���Ͻǵ�����
CvMat *H1; //�����ͼ֮��ı任����������ƽ�������и���

//����������ݲü����Ĵ���׼ͼ���е�Сͼ�ڲο�ͼ���е�λ�ã����������׼ͼ���ԭͼ�����Ͻ������ڲο�ͼ������ϵ�е�λ�ã�����˵�Ǵ���׼ͼ�����ϽǶ����ƽ������
bighx = smallhx / downrate;
bighy = smallhy / downrate;
//����ͼƽ����ת������ʾ�ڽ�����

//int h1 = abs(bighx) + 0.5;
//int h2 = abs(bighy) + 0.5;
//�Դ�ͼ��ı任�����е�ƽ�������и���
H1 = H;
cvmSet(H1,0,2,bighx);
cvmSet(H1,1,2,bighy);
cvmSet(H1,2,0,ha*downrate);
cvmSet(H1,2,1,hb*downrate);
t=(double)(cvGetTickCount()-t)/(cvGetTickFrequency()*1000*1000.); ///��ʱ����
//�õ��任����֮�������Ҫ����һ���µĸ����ͼ�����Դ洢������ͼ��׼�Ľ��

double h11,h12,h21,h22,sfx,sfy,rotate;

h11 = cvmGet(H1,0,0);
h12 = cvmGet(H1,0,1);
h21 = cvmGet(H1,1,0);
h22 = cvmGet(H1,1,1);
sfx = cvSqrt(h11*h11+h21*h21);
sfy = cvSqrt(h22*h22+h12*h12);
rotate = (180*atan(h21/h11))/3.1415;
CvPoint p1,p2,p3,p4; 
p2.x=bighx;  p2.y=bighy;

p1.x=(snimg[0]->width)*h11+bighx;  p1.y=h21*(snimg[0]->width)+bighy; 
p3.x=h12*(snimg[0]->height)+bighx; p3.y=h22*(snimg[0]->height)+bighy; 
p4.x=(snimg[0]->width)*h11+h12*(snimg[0]->height)+bighx; p4.y=h21*(snimg[0]->width)+h22*(snimg[0]->height)+bighy; 
