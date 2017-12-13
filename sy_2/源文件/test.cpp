H = H_2[0];

double smallhx,smallhy,ha,hb;///该参数表示两个裁剪出来的小图的平移参数
double bighx,bighy;//该参数表示待配准图像相对于参考图像的平移参数
//px = cvmGet( H, 0, 2 );
//py = cvmGet( H, 1, 2 );
smallhx = cvmGet( H, 0, 2 ); //这里算出的是待配准图像中感兴趣区域相对于参考图像的平移距离
smallhy = cvmGet( H, 1, 2 );
ha = cvmGet(H,2,0);
hb = cvmGet(H,2,1);
//现在还需要知道 参考图像中的小图相对于大图的位置或者坐标
//即参考图像中裁剪出的小图左上角的坐标
CvMat *H1; //定义大图之间的变换矩阵，尤其是平移量进行更新

//下面两句根据裁剪出的待配准图像中的小图在参考图像中的位置，计算出待配准图像的原图的左上角坐标在参考图像坐标系中的位置（或者说是待配准图像左上角顶点的平移量）
bighx = smallhx / downrate;
bighy = smallhy / downrate;
//将大图平移旋转参数显示在界面上

//int h1 = abs(bighx) + 0.5;
//int h2 = abs(bighy) + 0.5;
//对大图间的变换矩阵中的平移量进行更新
H1 = H;
cvmSet(H1,0,2,bighx);
cvmSet(H1,1,2,bighy);
cvmSet(H1,2,0,ha*downrate);
cvmSet(H1,2,1,hb*downrate);
t=(double)(cvGetTickCount()-t)/(cvGetTickFrequency()*1000*1000.); ///计时结束
//得到变换参数之后，下面就要建立一个新的更大的图，用以存储两个大图配准的结果

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
