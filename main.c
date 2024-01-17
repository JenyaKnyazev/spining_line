#define width_height 900
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
typedef struct _point{
	int x;
	int y;	
}point;
point* line;
point* line2;
int line_color=0xFF00;
int line_color2=0xFF0000;
int screen_color=0xFF;
SDL_Window* window=NULL;
SDL_Surface* surface=NULL;
SDL_Event event;
int move=1;
char flag=1;
int get_index(point *p){
	return p->x+p->y*width_height;
}
int get_index2(int x,int y){
	return x+y*width_height;
}
void init(){
	int i,*p,j;
	window=SDL_CreateWindow("Jenya line spin",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
	width_height,width_height,0);
	surface=SDL_GetWindowSurface(window);
	line=malloc(sizeof(point)*2);
	line2=malloc(sizeof(point)*2);
	line[0].x=450;
	line[0].y=450;
	line[1].x=630;
	line[1].y=450;
	line2[0].x=450;
	line2[0].y=450;
	line2[1].x=270;
	line2[1].y=450;
	p=surface->pixels;
	for(i=0;i<width_height*width_height;i++)
		p[i]=screen_color;
}


int absulute_value(int num){
	return num*(-1*(!!(num&(8<<(sizeof(num)*8-4)))))+num*(!(num&(8<<(sizeof(num)*8-4))));
}
int calc_x(int y){
	return round(sqrt(32400.0-(y-450)*(y-450))+450);	
}
int calc_y(int x){
	return round(sqrt(32400.0-(x-450)*(x-450))+450);
}
int calc_y2(int x){
	return round(450-sqrt(32400.0-(x-450)*(x-450)));
}
void move_line(int color,point* line_){
	int count=10,x,y,x2,y2,iy,ix,d;
	point* p=line_;
	int *p2;
	p2=surface->pixels;
	SDL_LockSurface(surface);
		if(flag){
			do{
				p[1].x-=move;
				p[1].y=calc_y(p[1].x);
			}while(p[1].x==630&&count--);
		}else{
			do{
				p[1].x+=move;
				p[1].y=calc_y2(p[1].x);
			}while(p[1].x==270&&count--);
		}
		if((p[1].x)==270||p[1].x==630)
			flag=!flag;
		x2=p[1].x;
		x=p[0].x;
		y2=p[1].y;
		y=p[0].y;
		x2-=x;
		y2-=y;
		iy=(y2>0)-(y2<0);
		ix=(x2>0)-(x2<0);
		x2=absulute_value(x2)*2;
		y2=absulute_value(y2)*2;
		if(x2>y2){
			d=y2-x2/2;
			while(x!=p[1].x){
				if(d>=0){
					d-=x2;
					y+=iy;
				}
				d+=y2;
				x+=ix;
				p2[get_index2(x,y)]=color;
			}
		}else{
			d=x2-y2/2;
			while(y!=p[1].y){
				if(d>=0){
					d-=y2;
					x+=ix;
				}
				d+=x2;
				y+=iy;
				p2[get_index2(x,y)]=color;
			}
		}
	
	SDL_UnlockSurface(surface);
	SDL_UpdateWindowSurface(window);
}
void clean_circle(){
	int* p=surface->pixels;
	int i,j;
	for(i=270;i<=630;i++)
		for(j=270;j<=630;j++)
			p[get_index2(i,j)]=screen_color;
}
void copy_line(point *p,point* p2){
	p->x=p2->x;
	p->y=p2->y;
	p++;
	p++;
	p->x=p2->x;
	p->y=p2->y;
}
int main(int argv,char **args){
	int save_move,i,j;
	init();
	while(1){
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					goto end;
			}
		}
		for(i=0;i<20;i++){
			move_line(line_color,line);
			flag=!flag;
			move_line(line_color2,line2);
			flag=!flag;
		}
		clean_circle();
		SDL_Delay(20);
		
	}
	end:
	free(line);
	free(line2);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
