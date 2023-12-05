#include <mlx.h>

# include <fcntl.h>
# include <math.h>
# include <mlx.h>
# include <stdio.h>
#include <stdlib.h>

# define HEIGHT 600
# define WIDTH 600

typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

typedef struct s_mlxinfo{
  void *mlx;
  void *mlx_win;
}t_mlxinfo;

int	close_win(t_mlxinfo *data)
{
	mlx_destroy_window(data->mlx, data->mlx_win);
	exit(1);
	return (0);
}

//内積を計算
double InnerProduct(double a[3], double b[3])
{
  return (a[0]*b[0] + a[1]*b[1] + a[2]*b[2]);
}

int argb_to_hex(int a, int rgb[3])
{
  int color;

  color = ((a & 0xFF) << 24) | ((rgb[0] & 0xFF) << 16) | ((rgb[1] & 0xFF) << 8) | (rgb[2] & 0xFF);
  return(color);
}

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	if (x > 0 && x < WIDTH && y > 0 && y < HEIGHT)
	{
		dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel
					/ 8));
		*(unsigned int *)dst = color;
	}
}

int main()
{
  t_mlxinfo data;
	t_data	img;
  int color_h;

  data.mlx = mlx_init();
	data.mlx_win = mlx_new_window(data.mlx, WIDTH, HEIGHT, "Hello world!");
	img.img = mlx_new_image(data.mlx, WIDTH , HEIGHT);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);

  //↓球体の設定
  // 視点座標
  double Pv[3] = {0.0, 0.0, -6000.0};
  // 中心座標
  double Pc[2][3] = {{-50.0, -50.0, -1000.0},
                    {-200.0, -200.0, -1700.0}};
  double R[2] = {600.0, 200.0};


  double Pvc[3];
  int k;

  double A, B2, C, D;

  double PL[3] = {20000,20000, -20000}; // 光源座標
  double P[3], N[3], vL[3], vR[3];
  double cosA;
  double t0;
  double t;

  double w, cosG;

  double v[3]; //視線方向ベクトル
  int i, j;

  double Kd = 1.0, Ks = 0.7, le = 0.1, l = 255.0;
  double RGB[2][3] = {{0, 0,205}, {0, 255, 0}};
  int RGB_back[3] = {176, 224,230};
  int color[3];

  for (i = -WIDTH; i <= (WIDTH - 1); i++){
    for (j = -HEIGHT; j <= (HEIGHT - 1); j++){
      int s0 = -1;
      for (int s=0; s<2; s++) {
        for (k=0; k<3; k++) Pvc[k] = Pv[k]-Pc[s][k];

        v[0] = i+0.5-Pv[0]; //x
        v[1] = j+0.5-Pv[1]; //y
        v[2] = 0-Pc[s][2]; //z
        A = InnerProduct(v, v);
        B2 = InnerProduct(Pvc, v);
        C = InnerProduct(Pvc, Pvc)-R[s]*R[s];
        D = B2*B2-A*C;
        if (D >= 0) {
          //視線と球の交点を求める
          t = (-B2-sqrt(D))/A;
          if (s0 < 0 || t < t0) {
            t0 = t;
            s0 = s;
          }
        }
        //点をかく
        if(s0 >= 0){
          for (k = 0; k<3; k++) P[k] = Pv[k] + v[k]*t0;
          // 法線
          for (k = 0; k<3; k++) N[k] = P[k] - Pc[s0][k];
          //光入射
          for (k = 0; k<3; k++) vL[k] = P[k] - PL[k];
          // cosAによって点の色（明るさ）が変化する
          // cosA < 0 のときはPは球自身の影にあるので黒色にする
          cosA = -InnerProduct(vL, N);
          cosA /= sqrt(InnerProduct(vL, vL) * InnerProduct(N, N));
          w = -(InnerProduct(vL, N)/InnerProduct(N, N));
          for(k=0; k<3; k++) vR[k] = 2*w*N[k]+vL[k];
          cosG = -InnerProduct(vR, v)/sqrt(InnerProduct(vR, vR)*InnerProduct(v,v));
          if(cosG < 0) cosG = 0;
            cosG = pow(cosG, 10); //10は鏡面度（10~20)
          for(k=0; k<3; k++)
          {
            color[k] = RGB[s0][k]*Kd*cosA; //拡散反射
            if(color[k] < l*Ks*cosG) color[k] = l*Ks*cosG;//鏡面反射は白色
            if(color[k] < RGB[s0][k]*le) color[k] = RGB[s0][k]*le; //le 環境光
          }
          color_h = argb_to_hex(0, color);
          my_mlx_pixel_put(&img, i + WIDTH/2, j + HEIGHT/2, color_h);
        }
        else{ //背景
          color_h = argb_to_hex(0, RGB_back);
          my_mlx_pixel_put(&img, i+ WIDTH/2, j+ HEIGHT/2, color_h);
        }
      }


      
    }
  }
  mlx_put_image_to_window(data.mlx, data.mlx_win, img.img, 0, 0);
  mlx_hook(data.mlx_win, 17, 0, close_win, &data);
  mlx_loop(data.mlx);
}

// int main()
// {
//   t_mlxinfo data;
// 	t_data	img;

//   data.mlx = mlx_init();
// 	data.mlx_win = mlx_new_window(data.mlx, WIDTH, HEIGHT, "Hello world!");
// 	img.img = mlx_new_image(data.mlx, WIDTH , HEIGHT);
// 	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);

//   double Pv[3] = {0.0, 0.0, 6000.0};
//   double Pc[3] = {0.0, 0.0, -3000.0};
//   double R = 600.0;

//   double Pvc[3];
//   int k;
//   for(k = 0; k<3; k++) Pvc[k] = Pv[k]-Pc[k];

//   double A, B2, C, D;

//   double PL[3] = {-60000,60000, 60000};
//   double P[3], N[3], vL[3];
//   double cosA;
//   double t0;

//   double v[3];
//   int i, j;
//   for (i = -600; i <= 599; i++){
//     for (j = -600; j <= 599; j++){
//       v[0] = i+0.5-Pv[0];
//       v[1] = j+0.5-Pv[1];
//       v[2] = 0-Pc[2];

//       A = InnerProduct(v, v);
//       B2 = InnerProduct(Pvc, v);
//       C = InnerProduct(Pvc, Pvc)-R*R;
//       D = B2*B2-A*C;

//       t0 = (-B2-sqrt(D))/A;
//       for (k = 0; k<3; k++) P[k] = Pv[k] + v[k]*t0;

//       for (k = 0; k<3; k++) N[k] = P[k] - Pc[k];
//       for (k = 0; k<3; k++) vL[k] = P[k] - PL[k];
//       cosA = -InnerProduct(vL, N);
//       cosA /= sqrt(InnerProduct(vL, vL) * InnerProduct(N, N));


//       if(D>= 0){
//         if(cosA >= 0)
//           my_mlx_pixel_put(&img, i + WIDTH/2, j + HEIGHT/2,0x000000DC * cosA);
//         else
//           my_mlx_pixel_put(&img, i+ WIDTH/2, j+ HEIGHT/2, 0);

//       }else{
//         my_mlx_pixel_put(&img, i+ WIDTH/2, j+ HEIGHT/2, 0x00add8e6);
//       }
//     }
//   }
//   mlx_put_image_to_window(data.mlx, data.mlx_win, img.img, 0, 0);
//   mlx_hook(data.mlx_win, 17, 0, close_win, &data);
//   mlx_loop(data.mlx);
// }
