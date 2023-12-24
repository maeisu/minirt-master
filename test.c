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
  double Pv[3] = {0.0, 0.0, -5.0};
  // 中心座標
  double Pc[3] = {0.0, 0.0, 5.0};
  double R = 1.0;


  double Pvc[3];
  int k;

  double A, B2, C, D;

  double PL[3] = {20000,20000, -20000}; // 光源座標
  double P[3], N[3], vL[3], vR[3];
  double cosA;
  double t0;
  double t;

  double w, cosG;

  double Pw[3]; //視線方向ベクトル
  int i, j;

  double Kd = 1.0, Ks = 0.7, le = 0.1, l = 255.0;
  double RGB[2][3] = {{0, 0,205}, {0, 255, 0}};
  int RGB_back[3] = {176, 224,230};
  int color[3] = {255,0,0};
  double De[3];
  double Vtmp[3];

  for (i = 0; i <= (HEIGHT - 1); i++){
    for (j = 0; j <= (WIDTH - 1); j++){

      Pw[0] = ((2*j)/(WIDTH-1)) - 1.0; //x
      Pw[1] = ((-2*i)/(HEIGHT-1)) + 1.0; //y
      Pw[2] = 0; //z

      De[0] = Pw[0] - Pv[0];
      De[1] = Pw[1] - Pv[1];
      De[2] = Pw[2] - Pv[2];

      Vtmp[0] = Pv[0] - Pc[0];
      Vtmp[1] = Pv[1] - Pc[1];
      Vtmp[2] = Pv[2] - Pc[2];

      A = InnerProduct(De, De);
      B2 = 2*InnerProduct(De, Vtmp);
      C = InnerProduct(Vtmp, Vtmp)-R*R;

      D = B2*B2-4*A*C;
      if (D >= 0) {
        color_h = argb_to_hex(0, color);
        my_mlx_pixel_put(&img, i, j, color_h);
      }
      else{ //背景
        color_h = argb_to_hex(0, RGB_back);
        my_mlx_pixel_put(&img, i, j, color_h);
      }
    }
  }
  mlx_put_image_to_window(data.mlx, data.mlx_win, img.img, 0, 0);
  mlx_hook(data.mlx_win, 17, 0, close_win, &data);
  mlx_loop(data.mlx);
}
