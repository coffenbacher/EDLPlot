/*
 * edlPlot.h
 *
 *  Created on: Apr 18, 2010
 *      Author: charlie
 */

#ifndef EDLPLOT_H_
#define EDLPLOT_H_

void plotAxes(SDL_Surface *screen, Uint32 c_colors[], int width, int height, int min_x, int max_x, int min_y, int max_y){
	//So, "0" needs to be min_x, "height" needs to be max_x
	//scale here
    int pixels_x = width;
	int pixels_y = height;
	int units_x = max_x - min_x;
    int units_y = max_y - min_y;

    //@NATMAT: THESE SHOULD BE USEFUL IN A TRANSLATION FUNCTION BETWEEN UNITS AND SCREEN PIXELS
    float pixels_per_unit_x = (float)pixels_x / (float)units_x;
    float pixels_per_unit_y = (float)pixels_y / (float)units_y;

    int x_axis_center = (-min_y)*pixels_per_unit_y;
    int y_axis_center = (-min_x)*pixels_per_unit_x;
    //Draw axes lines
	Draw_Line(screen, 0, x_axis_center, width, x_axis_center, c_colors[0]);
	Draw_Line(screen, y_axis_center, 0, y_axis_center, height, c_colors[0]);

	//Draw markers
	int i=1;
	while(i < width){
		Draw_Line(screen, i, x_axis_center-3, i, x_axis_center+3, c_colors[2]);
		i+= pixels_per_unit_x;
	}
	i=1;
	while(i < height){
		Draw_Line(screen, y_axis_center-3, i, y_axis_center+3, i, c_colors[2]);
		i+= pixels_per_unit_y;
	}
}

void plotLine(SDL_Surface *screen, Uint32 c_colors[]){
	  Draw_Line(screen, 100, 100, 30, 0, c_colors[1]);
	  Draw_Line(screen, 30,0, 50, 100, c_colors[4]);
};

#endif /* EDLPLOT_H_ */

