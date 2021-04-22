#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<float.h>
#include<math.h>
#include "myHeader.h"


int main(){
	CAD2D *cad=c2d_start_default();
	CAD2D *cad2=c2d_start_with_parameters(300,300);
	char *NULL_POINTER_FOR_TEXT=NULL;
	
	Hierarchy *h=NULL;
	double arr[][2]={{0,30},{0,0},{30,0},{30,30}};
	double arr2[][2]={{100,100},{100,150},{150,150},{150,100}};
	
	Point2D pp;
	pp.x=10;
	pp.y=15;
	h=c2d_create_hierarchy_as_root(cad);
	Hierarchy *kk;
	kk=c2d_create_hierarchy_as_root(cad2);

	CAD2D *cad3=c2d_start_to_the_hierarchy(300,300,h);
	Label k;
	strcpy(k.name,"WWW");
	Label x;
	strcpy(k.name,"W'''W");
	
	Point2D *head=create_linkedlist_of_points(arr2, 4);
	Point2D * bas=create_linkedlist_of_points(arr,4);
	Point2D *kafa=create_linkedlist_of_points(arr2,4);

	

	
	Label *la=c2d_add_polyline_label(cad,kafa,h,&k);
	Label *lt1=c2d_add_ellipse_label(cad,h,&x);
	Label *l2=c2d_add_text_label(cad,h,&k,NULL_POINTER_FOR_TEXT,0,0);

	Label *lta=c2d_add_circle_label(cad,h,&x);
	
	Label *ls=c2d_add_polygon_label(cad,bas,h,&x);
	
	Label *lt=c2d_add_arc(cad,1,1);

	Label *l1=c2d_add_pointlabel(cad,&pp,h,&k); 
	
	


	

	c2d_export(cad,"new","bin");
	

	cad2=c2d_import("new","bin");


	c2d_export(cad2,"out","eps");

	


}