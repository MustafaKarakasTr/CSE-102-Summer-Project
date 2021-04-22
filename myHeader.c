#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<float.h>
#include<math.h>
#include "myHeader.h"


//#define M_PI acos(-1.0);


CAD2D *c2d_start_default(){ //only all hierarchy's first objects should call this function
									  // others will be involved in this canvas hopefully
	CAD2D *node=(CAD2D *)malloc(sizeof(CAD2D));
	node->width=CANVAS_SIZE_without_parameter;
	node->height=CANVAS_SIZE_without_parameter;
	node->Points_of_objects=NULL;
	node->next_object==NULL;

	return node;
}
CAD2D *c2d_start_with_parameters(double width,double height){
	CAD2D *node=(CAD2D *)malloc(sizeof(CAD2D));
	node->width=width;
	node->height=height;
	node->Points_of_objects=NULL;
	node->next_object==NULL;
	return node;
}
CAD2D *c2d_start_to_the_hierarchy(double width,double height, Hierarchy *h){
	h->object=(CAD2D *)malloc(sizeof(CAD2D));
	h->object->width=width;
	h->object->height=height;
	h->object->Points_of_objects=NULL;
	h->object->next_object=NULL;
	//no need to return anything because we already put the CAD2D object to the given hierarchy
	//but I will return it anyway
	return h->object;
}

Hierarchy* add_child(CAD2D *cad,Hierarchy *head){



	
	Hierarchy *temp=head;
	if(head==NULL){
		head=(Hierarchy*)malloc(sizeof(Hierarchy));
		head->object=cad;
		head->child=NULL;
		head->sibling=NULL;
	}
	else if(temp->child==NULL){
		temp->child=(Hierarchy*)malloc(sizeof(Hierarchy));
		temp->child->object=cad;
		temp->child->child=NULL;
		temp->child->sibling=NULL;
	}
	else{
		temp=head->child;
		while(temp->sibling!=NULL){
			temp=temp->sibling;
		}
		temp->sibling=(Hierarchy*)malloc(sizeof(Hierarchy));
		temp->sibling->object=cad;
		temp->sibling->sibling=NULL;
		temp->sibling->child=NULL;
	}
	return head;

}
void print_Tree(Hierarchy *head){
	Hierarchy* temp=head;
	if(temp==NULL)
		return;
	//printf("%d ",head->data );
	print_Tree(head->sibling);
	print_Tree(head->child);
	
	
}
void add_hie(Hierarchy *h){
	h->child=(Hierarchy *)malloc(sizeof(Hierarchy));
	h->child->object=(CAD2D*)malloc(sizeof(CAD2D));
	h->child->object->Points_of_objects=(Point2D*)malloc(sizeof(Point2D));
	h->child->object->Points_of_objects->x=22;
}
void print_value_of_hie(Hierarchy *h){
	if(h==NULL)
		return;
	CAD2D *temp=h->object;
	while(temp!=NULL)
	{
		while(temp->Points_of_objects!=NULL){
			printf("%lf %lf\n",temp->Points_of_objects->x,temp->Points_of_objects->y);
			temp->Points_of_objects=temp->Points_of_objects->next;
		}
		temp=temp->next_object;
	}
	print_value_of_hie(h->sibling);
	
	print_value_of_hie(h->child);

}
void my_free(Hierarchy *root){
	if(root==NULL)
		return;
	my_free(root->child);
	my_free(root->sibling);
	if(root->object!=NULL)
		free(root->object);
	free(root);
}
void print_points(CAD2D *cad){
	Point2D* point;
	if( cad==NULL || cad->Points_of_objects==NULL ){//if cad equals to NULL Hopefully it wont check if cad->Points_of_objects equals to NULL or not if it does seg fault will appear
		return;
	}

	point = cad->Points_of_objects;
	if(cad->next_object==NULL){
		return;
	}
	printf("\n");
	print_points(cad->next_object);
}
void print_point(CAD2D *cad){
	if(cad==NULL)
		return;
	printf("%lf %lf \n",cad->Points_of_objects->x,cad->Points_of_objects->y);
	print_point(cad->next_object);
}
Point2D *create_linkedlist_of_points(double arr[][2], int size){
	Point2D *p=NULL;
	int x=1;
	Point2D *temp=NULL;
	// bu fonksiyon linked listi olusturup head'ini dondurcek ınsallah
	p=(Point2D*)malloc(sizeof(Point2D));
	p->x=arr[0][0];
	p->y=arr[0][1];
	temp=p;
	while(x<size){
		temp->next=(Point2D*)malloc(sizeof(Point2D));
		temp->next->x=arr[x][0];
		temp->next->y=arr[x][1];
		temp=temp->next;
		temp->next=NULL;
		x++;
	}
	return p;
}
void free_point(Point2D *temp){
	if(temp==NULL)
		return;
	free_point(temp->next);
	free(temp);
}	

void print_text(Hierarchy *h){
	if(h==NULL)
		return;
	CAD2D *t=h->object;
	while(t->next_object !=NULL){
		if(h->object->type_of_object == Text){
			printf("%s\n",h->object->text );
			
		}
		t=t->next_object;
		

	}
	print_text(h->child);
	print_text(h->sibling);
}



int make_sure_label_is_unique(char *s){
	FILE *fp=fopen("labels.txt","r");
	int d=1;
	char readed[MAX_LABEL_LENGTH];

	if(fp!=NULL){
		while(d!=EOF ){

			d=fscanf(fp,"%s",readed);
			if(d==EOF){
				break;
			}
			if(!strcmp(s,readed)){
				//if readed equals to writed label then we should take another name for label
				return 0;
			}

		}
		fclose(fp);
	}
	fp=fopen("labels.txt","a");
	fprintf(fp, "%s\n",s );
	fclose(fp);
	//printf("1%s1",s );	
	return 1;
}
Label* c2d_add_point_xy(CAD2D *cad,double x,double y){
	char label_of_cad2d[MAX_LABEL_LENGTH];
	Point2D *point;
	CAD2D *object;
	CAD2D *temp;
	int count=0;
	Label *l=(Label *)malloc(sizeof(Label));
	do{
		if(count>0){
			printf("This label is taken.Please try again\n");
		}
		printf("Enter the label of the point: ");
		scanf("%s",label_of_cad2d);	
		count++;
	
	}while(make_sure_label_is_unique(label_of_cad2d)==0);
	if(cad->Points_of_objects==NULL){//there is no object in this cad
		point=(Point2D*)malloc(sizeof(Point2D));
		point->x=x;
		point->y=y;
		point->next=NULL;
		cad->Points_of_objects=point;
		strcpy(l->name,label_of_cad2d);
		l->cad=cad;


	}
	else{
		object=cad;
		while(object->next_object!=NULL){
			object=object->next_object;
		}
		temp=(CAD2D*)malloc(sizeof(CAD2D));
		temp->next_object=NULL;
		point=(Point2D*)malloc(sizeof(Point2D));
		point->x=x;
		point->y=y;
		point->next=NULL;



		object->next_object=temp;
		object->next_object->Points_of_objects=point;
		object=object->next_object;
		l->cad=object;
		strcpy(l->name,label_of_cad2d);
		


	}
	l->x_of_center=x;
	l->y_of_center =y;
	l->type_of_object=Point;
	return l;

}

Label* c2d_add_point_point(CAD2D *cad,Point2D *p){
	char label_of_cad2d[MAX_LABEL_LENGTH];
	Point2D *point;
	int d=0;
	CAD2D *object;
	CAD2D *temp;
	Label *l=(Label *)malloc(sizeof(Label));
	l->x_of_center=p->x;
	l->y_of_center =p->y;
	do{
		if(d>0)
			printf("This Label has already been taken ,Please try again:\n");
		printf("Enter the label of the point: ");
		scanf("%s",label_of_cad2d);	
		
		d++;
	}while(make_sure_label_is_unique(label_of_cad2d)==0);

	if(cad->Points_of_objects==NULL){//there is no object in this cad
		point=(Point2D*)malloc(sizeof(Point2D));
		point->x=p->x;
		point->y=p->y;
		point->next=NULL;
		cad->Points_of_objects=point;
		l->cad=cad;
		
	}
	else{
		object=cad;
		while(object->next_object!=NULL){
			object=object->next_object;
		}
		temp=(CAD2D*)malloc(sizeof(CAD2D));
		temp->next_object=NULL;
		point=(Point2D*)malloc(sizeof(Point2D));
		point->x=p->x;
		point->y=p->y;
		point->next=NULL;

			

		object->next_object=temp;
		object->next_object->Points_of_objects=point;
		object=object->next_object;
		l->cad=object;

		


	}
	strcpy(l->name,label_of_cad2d);
	l->type_of_object=Point;
	return l;

}
Label* c2d_add_point_hierarchy(CAD2D *cad,Point2D *p,Hierarchy *h){
	Label *l=c2d_add_point_point(cad,p);

	
	Hierarchy *head=h;
	Hierarchy *temp=head;
	/*if(head==NULL)
		printf("A\n");
	else{
		printf("b\n");
	}*/
	if(head==NULL){
		head=(Hierarchy*)malloc(sizeof(Hierarchy));
		//head->data=x;
		head->child=NULL;
		head->sibling=NULL;
		h=head;
		temp=head;
	}
	else if(temp->child==NULL){
		temp->child=(Hierarchy*)malloc(sizeof(Hierarchy));
		//temp->child->data=x;
		temp->child->child=NULL;
		temp->child->sibling=NULL;

		temp=temp->child;
		h->child=temp;
	}
	else{

		temp=head->child;
		while(temp->sibling!=NULL){
			temp=temp->sibling;
		}
		temp->sibling=(Hierarchy*)malloc(sizeof(Hierarchy));
		//temp->sibling->data=x;
		temp->sibling->sibling=NULL;
		temp->sibling->child=NULL;
		temp=temp->sibling;

	}
	temp->object=cad;// given cad has been attached to the given hierarchy's child
	l->type_of_object=Point;
	return l;
	
}
Label* c2d_add_pointlabel(CAD2D *cad,Point2D *p,Hierarchy *h,Label * l){
	char label_of_cad2d[MAX_LABEL_LENGTH];
	Point2D *point;
	CAD2D *object;
	CAD2D *tem;
	Hierarchy *head=h;
	Hierarchy *temp=head;

	if(cad->Points_of_objects==NULL){//there is no object in this cad
		point=(Point2D*)malloc(sizeof(Point2D));
		point->x=p->x;
		point->y=p->y;
		point->next=NULL;
		cad->Points_of_objects=point;
		l->cad=cad;
		

	}
	else{
		object=cad;
		while(object->next_object!=NULL){
			object=object->next_object;
		}
		tem=(CAD2D*)malloc(sizeof(CAD2D));
		tem->next_object=NULL;
		point=(Point2D*)malloc(sizeof(Point2D));
		point->x=p->x;
		point->y=p->y;
		point->next=NULL;

		

		object->next_object=tem;
		object->next_object->Points_of_objects=point;
		object=object->next_object;
		l->cad=object;
		
		


	}
	
	/*if(head==NULL)
		printf("A\n");
	else{
		printf("b\n");
	}*/
	if(head==NULL){
		head=(Hierarchy*)malloc(sizeof(Hierarchy));
		//head->data=x;
		head->child=NULL;
		head->sibling=NULL;
		h=head;
		temp=head;
	}
	else if(temp->child==NULL){
		temp->child=(Hierarchy*)malloc(sizeof(Hierarchy));
		//temp->child->data=x;
		temp->child->child=NULL;
		temp->child->sibling=NULL;

		temp=temp->child;
		h->child=temp;

	}
	else{

		temp=head->child;
		while(temp->sibling!=NULL){
			temp=temp->sibling;
		}
		temp->sibling=(Hierarchy*)malloc(sizeof(Hierarchy));
		//temp->sibling->data=x;
		temp->sibling->sibling=NULL;
		temp->sibling->child=NULL;
		temp=temp->sibling;

	}
	temp->object=cad;// given cad has been attached to the given hierarchy's child
	l->type_of_object=Point;
	l->x_of_center= p->x;
	l->y_of_center=p->y;
	return l;
}

Hierarchy *c2d_create_hierarchy_as_root(CAD2D *cad){
	Hierarchy *root=(Hierarchy*)malloc(sizeof(Hierarchy));
	root->object=cad;
	root->child=NULL;
	root->sibling=NULL;
	return root;
}
Hierarchy *c2d_create_hierarchy_as_child(CAD2D *cad,Hierarchy *parent){
	Hierarchy *temp=parent;
	if(parent==NULL){
		parent=(Hierarchy*)malloc(sizeof(Hierarchy));
		parent->object=cad;
		parent->child=NULL;
		parent->sibling=NULL;
	}
	else if(temp->child==NULL){
		temp->child=(Hierarchy*)malloc(sizeof(Hierarchy));
		temp->child->object=cad;
		temp->child->child=NULL;
		temp->child->sibling=NULL;
	}
	else{
		temp=parent->child;
		while(temp->sibling!=NULL){
			temp=temp->sibling;
		}
		temp->sibling=(Hierarchy*)malloc(sizeof(Hierarchy));
		temp->sibling->object=cad;
		temp->sibling->sibling=NULL;
		temp->sibling->child=NULL;
	}
	return parent;

}

Label *c2d_add_polyline_xy(CAD2D *cad,double points[][2],int number_of_edges){
	int i=0;
	int d=0;
	char label_of_cad2d[MAX_LABEL_LENGTH];
	CAD2D *temp=cad;
	Point2D *p;
	char name[10];
	Point2D *Point;
	Label *l=(Label *)malloc(sizeof(Label));
	do{
		if(d>0)
			printf("This Label has already been taken ,Please try again:\n" );
			printf("Enter the label of the polyline: ");
			scanf("%s",label_of_cad2d);	
			d++;//counter for print error message
		}while(make_sure_label_is_unique(label_of_cad2d)==0);
	if(temp==NULL){

		printf("Please call one of the c2d_start_? functions before calling c2d_add_polyline_xy function\n" );

	}

	else if(temp->Points_of_objects==NULL){//it will create linked list of point2d
		temp->Points_of_objects=(Point2D*)malloc(sizeof(Point2D));
		temp->Points_of_objects->x=points[i][0];
		temp->Points_of_objects->y=points[i][1];
		Point=temp->Points_of_objects;
		i++;

		for(;i<number_of_edges;i++){
			Point->next=(Point2D*)malloc(sizeof(Point2D));
			Point->next->x=points[i][0];
			Point->next->y=points[i][1];
			Point=Point->next;
			
		}

		Point->next=NULL;
		strcpy(l->name,label_of_cad2d);
		l->cad=temp;
		cad->next_object=NULL;

	}
	else{
		while(temp->next_object!=NULL){
			temp=temp->next_object;

		}
		temp->next_object=(CAD2D*)malloc(sizeof(CAD2D));
		temp->next_object->Points_of_objects=(Point2D*)malloc(sizeof(Point2D));
		Point=temp->next_object->Points_of_objects;
		Point->x=points[0][0];
		Point->y=points[0][1];
		i++;
		for(;i<number_of_edges;i++){
			Point->next=(Point2D*)malloc(sizeof(Point2D));
			Point->next->x=points[i][0];
			Point->next->y=points[i][1];
			Point=Point->next;
			
		}
		Point->next=NULL;
		/*
		do{
			printf("Enter the label of the point: ");
			scanf("%s",label_of_cad2d);	
			if(d>0)
				printf("This Label has already been taken ,Please try again:\n");
			d++;
		
		}while(make_sure_label_is_unique(label_of_cad2d)==0);
		*/
		strcpy(l->name,label_of_cad2d);
		l->cad=temp->next_object;
		temp->next_object->next_object=NULL;

	}
	l->type_of_object=Polyline;
	return l;
	
}
Label *c2d_add_polyline_point(CAD2D* cad,Point2D *p){// p is head of linked list
	Point2D *point;
	CAD2D *temp;
	Label *l=(Label *)malloc(sizeof(Label));
	int d=0;
	char label_of_cad2d[MAX_LABEL_LENGTH];
	do{
		if(d>0)
			printf("This Label has already been taken ,Please try again:\n" );
			printf("Enter the label of the polyline: ");
			scanf("%s",label_of_cad2d);	
			d++;//counter for print error message
	}while(make_sure_label_is_unique(label_of_cad2d)==0);


	if(cad==NULL){
		printf("Please call one of the c2d_start_? functions before calling c2d_add_polyline_point function  \n");
	}
	else if(cad->Points_of_objects==NULL){//if given cad entity does not have any point
		cad->Points_of_objects=p;
		l->cad=cad;
		strcpy(l->name,label_of_cad2d);
		cad->next_object=NULL;


	}
	else{ // if given cad entity has points 
		temp=cad;
		while(temp->next_object!=NULL){
			temp=temp->next_object;
		}
		temp->next_object=(CAD2D*)malloc(sizeof(CAD2D));
		temp->next_object->Points_of_objects=p;
		temp->next_object->next_object=NULL;
	
		l->cad=temp->next_object;
		strcpy(l->name,label_of_cad2d);

	}
	l->type_of_object=Polyline;
	return l;
}
Label *c2d_add_polyline_hierarchy(CAD2D *cad,Point2D *p,Hierarchy *h){
	Label *l=c2d_add_polyline_point(cad,p);
	Hierarchy *head=h;
	Hierarchy *temp=head;
	if(head==NULL){
		head=(Hierarchy*)malloc(sizeof(Hierarchy));
		//head->data=x;
		head->child=NULL;
		head->sibling=NULL;
		h=head;
		temp=head;
	}
	else if(temp->child==NULL){
		temp->child=(Hierarchy*)malloc(sizeof(Hierarchy));
		//temp->child->data=x;
		temp->child->child=NULL;
		temp->child->sibling=NULL;

		temp=temp->child;
		h->child=temp;
	}
	else{

		temp=head->child;
		while(temp->sibling!=NULL){
			temp=temp->sibling;
		}
		temp->sibling=(Hierarchy*)malloc(sizeof(Hierarchy));
		//temp->sibling->data=x;
		temp->sibling->sibling=NULL;
		temp->sibling->child=NULL;
		temp=temp->sibling;

	}
	temp->object=cad;// given cad has been attached to the given hierarchy's child
	l->type_of_object=Polyline;
	return l;
}
Label *c2d_add_polyline_label(CAD2D *cad,Point2D *p,Hierarchy *h,Label *l){
	Point2D *point;
	CAD2D *object;
	CAD2D *tem;
	Hierarchy *head=h;
	Hierarchy *temp=head;

	if(cad->Points_of_objects==NULL){
		cad->Points_of_objects=p;
		l->cad=cad;
	}
	else{
		object=cad;
		while(object->next_object!=NULL){

			object=object->next_object;
		}
		object->next_object=(CAD2D*)malloc(sizeof(CAD2D));
		object=object->next_object;
		object->next_object=NULL;
		object->Points_of_objects=p;
		l->cad=object;	
	}
	if(head==NULL){
		head=(Hierarchy*)malloc(sizeof(Hierarchy));
		//head->data=x;
		head->child=NULL;
		head->sibling=NULL;
		h=head;
		temp=head;
	}
	else if(temp->child==NULL){
		temp->child=(Hierarchy*)malloc(sizeof(Hierarchy));
		//temp->child->data=x;
		temp->child->child=NULL;
		temp->child->sibling=NULL;

		temp=temp->child;
		h->child=temp;
	}
	else{

		temp=head->child;
		while(temp->sibling!=NULL){
			temp=temp->sibling;
		}
		temp->sibling=(Hierarchy*)malloc(sizeof(Hierarchy));
		//temp->sibling->data=x;
		temp->sibling->sibling=NULL;
		temp->sibling->child=NULL;
		temp=temp->sibling;

	}
	temp->object=cad;// given cad has been attached to the given hierarchy's child
	l->type_of_object=Polyline;
	return l;
}
Label *c2d_add_polygon_xy(CAD2D *cad,double points[][2],int number_of_edges){
	int i=0;
	int d=0;
	char label_of_cad2d[MAX_LABEL_LENGTH];
	CAD2D *temp=cad;
	Point2D *p;
	char name[10];
	Point2D *Point;
	Label *l=(Label *)malloc(sizeof(Label));
	if(number_of_edges>2){//polygon should have more than 2 edges
		do{
			if(d>0)
				printf("This Label has already been taken ,Please try again:\n" );
				printf("Enter the label of the polyline: ");
				scanf("%s",label_of_cad2d);	
				d++;//counter for print error message
			}while(make_sure_label_is_unique(label_of_cad2d)==0);
		
		if(temp==NULL){

			printf("Please call one of the c2d_start_? functions before calling c2d_add_polygon_xy function\n" );

		}
		
		else{
			while(temp->next_object!=NULL){
				temp=temp->next_object;
			}
			temp->next_object=(CAD2D*)malloc(sizeof(CAD2D));
			temp->next_object->Points_of_objects=(Point2D*)malloc(sizeof(Point2D));
			Point=temp->next_object->Points_of_objects;
			Point->x=points[0][0];
			Point->y=points[0][1];
			i++;
			for(;i<number_of_edges;i++){
				Point->next=(Point2D*)malloc(sizeof(Point2D));
				Point->next->x=points[i][0];
				Point->next->y=points[i][1];
				Point=Point->next;
				
			}
			Point->next=(Point2D*)malloc(sizeof(Point2D));
			Point->next->x=points[0][0];
			Point->next->y=points[0][1];
			Point->next->next=NULL;
			/*
			do{
				printf("Enter the label of the point: ");
				scanf("%s",label_of_cad2d);	
				if(d>0)
					printf("This Label has already been taken ,Please try again:\n");
				d++;
			
			}while(make_sure_label_is_unique(label_of_cad2d)==0);
			*/
			l->cad=temp->next_object;
			strcpy(l->name,label_of_cad2d);
			p=compute2DPolygonCentroid(temp->next_object->Points_of_objects);
			l->x_of_center=p->x;
			l->y_of_center=p->y;
			temp->next_object->next_object=NULL;

		}
	}
	l->type_of_object=Polygon;

	return l;
}
Label *c2d_add_polygon_point(CAD2D* cad,Point2D *p){
	Point2D *point;
	CAD2D *temp;
	Point2D *p1;
	int d=0;
	double x,y;
	char label_of_cad2d[MAX_LABEL_LENGTH];
	Label *l=(Label *)malloc(sizeof(Label));
	point = p;
	x=point->x;
	y=point->y;
	if(point !=NULL){
		while(point->next!=NULL){
			d++;
			point=point->next;
		}
		d++;
		
		point->next=(Point2D*)malloc(sizeof(Point2D));
		point->next->x=x;
		point->next->y=y;

		point->next->next=NULL;
		if(d>2){
			d=0;
		 	do{
				if(d>0)
					printf("This Label has already been taken ,Please try again:\n" );
				printf("Enter the label of the polygon: ");
				scanf("%s",label_of_cad2d);	
				d++;//counter for print error message
			}while(make_sure_label_is_unique(label_of_cad2d)==0);

			
			if(cad==NULL){
				printf("Please call one of the c2d_start_? functions before calling c2d_add_polyline_point function  \n");
			}
			else if(cad->Points_of_objects==NULL){//if given cad entity does not have any point
				cad->Points_of_objects=p;
				l->cad=cad;
				strcpy(l->name,label_of_cad2d);
				cad->type_of_object = Polygon;
			}
			else{ // if given cad entity has points 
				temp=cad;
				while(temp->next_object!=NULL){
					temp=temp->next_object;
				}
				temp->next_object=(CAD2D*)malloc(sizeof(CAD2D));
				temp->next_object->Points_of_objects=p;
				temp->next_object->type_of_object = Polygon;
				temp->next_object->next_object=NULL;
			
			l->cad=temp->next_object;
			strcpy(l->name,label_of_cad2d);

			}
		}
	}
	l->type_of_object=Polygon;
	p1=compute2DPolygonCentroid(p);
	l->x_of_center=p1->x;
	l->y_of_center=p1->y;
	return l;
}
Label *c2d_add_polygon_hierarchy(CAD2D *cad,Point2D *p,Hierarchy *h){
	
	CAD2D *qw =cad;
	Label *l;
	Hierarchy *head=h;
	Hierarchy *temp=head;
	l=c2d_add_polygon_point(cad,p);
	if(head==NULL){
		head=(Hierarchy*)malloc(sizeof(Hierarchy));
		//head->data=x;
		head->child=NULL;
		head->sibling=NULL;
		h=head;
		temp=head;
	}
	else if(temp->child==NULL){
		temp->child=(Hierarchy*)malloc(sizeof(Hierarchy));
		//temp->child->data=x;
		temp->child->child=NULL;
		temp->child->sibling=NULL;

		temp=temp->child;
		h->child=temp;
	}
	else{

		temp=head->child;
		while(temp->sibling!=NULL){
			temp=temp->sibling;
		}
		temp->sibling=(Hierarchy*)malloc(sizeof(Hierarchy));
		//temp->sibling->data=x;
		temp->sibling->sibling=NULL;
		temp->sibling->child=NULL;
		temp=temp->sibling;

	}
	temp->object=cad;
	l->type_of_object=Polygon;
	return l;

}
Label *c2d_add_polygon_label(CAD2D *cad,Point2D *p,Hierarchy *h,Label *l){
	Point2D *point=p;
	CAD2D *object;
	CAD2D *tem;
	Point2D *center;
	double x,y;
	Hierarchy *head=h;
	Hierarchy *temp=head;
	object=cad;
	if(p!=NULL){
		x=p->x;
		y=p->y;
	}
	while(point->next!=NULL){
		point=point->next;
	}
	point->next=(Point2D *)malloc(sizeof(Point2D));
	point->next->x=x;
	point->next->y=y;
	point->next->next=NULL;
	if(cad->Points_of_objects==NULL){
		cad->Points_of_objects=p;
		l->cad=cad;
	}
	else{
		object=cad;
		while(object->next_object!=NULL){
			object=object->next_object;
		}
		object->next_object=(CAD2D*)malloc(sizeof(CAD2D));
		object=object->next_object;
		object->next_object=NULL;
		object->Points_of_objects=p;
		l->cad=object;	
	}
	if(head==NULL){
		head=(Hierarchy*)malloc(sizeof(Hierarchy));
		//head->data=x;
		head->child=NULL;
		head->sibling=NULL;
		h=head;
		temp=head;
	}
	else if(temp->child==NULL){
		temp->child=(Hierarchy*)malloc(sizeof(Hierarchy));
		//temp->child->data=x;
		temp->child->child=NULL;
		temp->child->sibling=NULL;

		temp=temp->child;
		h->child=temp;
	}
	else{

		temp=head->child;
		while(temp->sibling!=NULL){
			temp=temp->sibling;
		}
		temp->sibling=(Hierarchy*)malloc(sizeof(Hierarchy));
		//temp->sibling->data=x;
		temp->sibling->sibling=NULL;
		temp->sibling->child=NULL;
		temp=temp->sibling;

	}
	temp->object=cad;// given cad has been attached to the given hierarchy's child
	//l->cad=temp->object;
	l->type_of_object=Polygon;
	center=compute2DPolygonCentroid(p);
	l->x_of_center=center->x;
	l->y_of_center=center->y;
	return l;
}
char * take_label(char *s){
	int d=0;
	//char label_of_cad2d[MAX_LABEL_LENGTH];
	char *label_of_cad2d=(char *)malloc(sizeof(char)*MAX_LABEL_LENGTH);

	do{
		if(d>0)
			printf("This Label has already been taken ,Please try again:\n" );
		printf("Enter the label of the %s: ",s);
		scanf("%s",label_of_cad2d);	
		d++;//counter for print error message
	}while(make_sure_label_is_unique(label_of_cad2d)==0);

	return label_of_cad2d;

}
void add_Point2D_to_given_cad_entity(CAD2D *cad,Point2D* points, Label *label){
	CAD2D *object=NULL,*temp=NULL;
	if(cad->Points_of_objects==NULL){//there is no object in this cad
		
		cad->Points_of_objects=points;
		label->cad=cad;
		cad->next_object=NULL;

	}
	else{
		object=cad;
		while(object->next_object!=NULL){
			object=object->next_object;
		}
		temp=(CAD2D*)malloc(sizeof(CAD2D));
		temp->next_object=NULL;
		

		//temp->label=(Label*)malloc(sizeof(Label));
		
		//strcpy(temp->label->name,label->name);
		

		object->next_object=temp;
		object->next_object->Points_of_objects=points;
		//object->next_object->label=temp->label;
		label->cad=temp;

		object=object->next_object;
		object->next_object=NULL;
		//printf("%lf %lf %s\n",object->Points_of_objects->x,object->Points_of_objects->y,object->label->name);
		

	}
}

Point2D *create_arc_between_given_angles(double x_of_center,double y_of_center,double diameter,double start_angle,double end_angle){
	Point2D *head=NULL,*iter=NULL;

	double x_of_point;
	double y_of_point;

	/*
	while(start_angle>359){
		start_angle-=360;
	}
	while(end_angle>359){
		end_angle-=360;
	}
	if(start_angle>359)
		start_angle=359;
	*/
	if(diameter<=0){
		printf("Diameter should be bigger than 0\n");
	}
	else{

		head=(Point2D*)malloc(sizeof(Point2D));
		iter=head;
		x_of_point=diameter*sin(start_angle*M_PI/180)+x_of_center;
		y_of_point=diameter*cos(start_angle*M_PI/180)+y_of_center;
		iter->x=x_of_point;
		iter->y=y_of_point;

		//while(!(start_angle+0.0001>end_angle && start_angle-0.0001<end_angle)){
		if(start_angle<end_angle){
			while(start_angle<=end_angle){
				iter->next=(Point2D*)malloc(sizeof(Point2D));
				iter=iter->next;
				x_of_point=diameter*sin(start_angle*M_PI/180)+x_of_center;
				y_of_point=diameter*cos(start_angle*M_PI/180)+y_of_center;
				iter->x=x_of_point;
				iter->y=y_of_point;
				iter->next=NULL;
				/*
				x_of_point=diameter*sin(start_angle*M_PI/180)+x_of_center;
				y_of_point=diameter*cos(start_angle*M_PI/180)+y_of_center;
				iter->x=x_of_point;
				iter->y=y_of_point;
				*/
				


				//printf("%lf %lf %lf %lf\n",x_of_point,y_of_point, cos(start_angle*M_PI/180),sin(start_angle*M_PI/180));
				start_angle++;
				
			}
		}
		else{
			while(start_angle<360){
				iter->next=(Point2D*)malloc(sizeof(Point2D));
				iter=iter->next;
				x_of_point=diameter*sin(start_angle*M_PI/180)+x_of_center;
				y_of_point=diameter*cos(start_angle*M_PI/180)+y_of_center;
				iter->x=x_of_point;
				iter->y=y_of_point;
				iter->next=NULL;
				start_angle++;
			}
			start_angle=0;
			while(start_angle<=end_angle){
				iter->next=(Point2D*)malloc(sizeof(Point2D));
				iter=iter->next;
				x_of_point=diameter*sin(start_angle*M_PI/180)+x_of_center;
				y_of_point=diameter*cos(start_angle*M_PI/180)+y_of_center;
				iter->x=x_of_point;
				iter->y=y_of_point;
				iter->next=NULL;
				/*
				x_of_point=diameter*sin(start_angle*M_PI/180)+x_of_center;
				y_of_point=diameter*cos(start_angle*M_PI/180)+y_of_center;
				iter->x=x_of_point;
				iter->y=y_of_point;
				*/
				


				//printf("%lf %lf %lf %lf\n",x_of_point,y_of_point, cos(start_angle*M_PI/180),sin(start_angle*M_PI/180));
				start_angle++;
				
			}
		}

	}
	return head;
}
Label *c2d_add_arc(CAD2D *cad,int this_parameter_should_be_1,int this_parameter_should_be_1_again){//in the c2d_add_arc_label(...) function I'll call this function and when I do, I'll make int parameter equal to zero because I dont want to take label again in that function
	Point2D *head_of_points=NULL,*iter=NULL;
	char s[10];
	double x,y,diameter,start_angle,end_angle;
	Label *label=(Label *)malloc(sizeof(Label));
	label->type_of_object=Arc;
	printf("Enter the x value of center of your ");
	if(this_parameter_should_be_1_again){
		printf("arc:");
		strcpy(s,"arc");
	}
	else{
		printf("circle:");
		strcpy(s,"circle");
	}
	scanf("%lf",&x);
	printf("Enter the y value of center of your ");
	if(this_parameter_should_be_1_again){
		printf("arc:");
	}
	else{
		printf("circle:");
	}
	
	scanf("%lf",&y);
	label->x_of_center=x;
	label->y_of_center=y;
	printf("Enter the diameter :");
	scanf("%lf",&diameter);
	// When I draw a circle I'll call this function and this_parameter_should_be_1_again will be 0 so start_angle and end_angle will not change
	start_angle=0;
	end_angle=360;

	if(this_parameter_should_be_1_again){
		// I am planning to use this function when we draw circle so I added this parameter ,when I call for circle I will make this_parameter_should_be_1_again 0
		printf("Enter the start angle (clockwise first): ");
		scanf("%lf",&start_angle);
		printf("Enter the end angle (clockwise last): ");
		scanf("%lf",&end_angle);

	}
	head_of_points=create_arc_between_given_angles(x,y,diameter,start_angle,end_angle);// it returns head of Point2D linked list which contains coordinates of arc
	if(this_parameter_should_be_1){//when we implement c2d_add_arc_label function I will call this function but this_parameter_should_be_1 will be 0 so I will not take label from user
		strcpy(label->name,take_label(s));
	}
	/*iter=head_of_points;
	while(iter!=NULL){
		printf("%lf %lf\n",iter->x,iter->y );
		iter=iter->next;
	}*/
	add_Point2D_to_given_cad_entity(cad,head_of_points,label);
	//printf("%lf %lf %lf %lf %s\n",x,y,start_angle,end_angle,label.name );
	return label;
}

Label *c2d_add_arc_hierarchy(CAD2D *cad,Hierarchy *h){
	Label *l =c2d_add_arc(cad,1,1);
	h=add_child(cad,h);
	return l;
}
Label *c2d_add_arc_label(CAD2D *cad,Hierarchy *h,Label *l){
	CAD2D *temp;
	Label *label_temp;
	label_temp=c2d_add_arc(cad,0,1);
	temp=cad;

	while(temp->next_object!=NULL){
		temp=temp->next_object;
	}
	//temp->label=l;
	l->cad=temp;
	l->x_of_center=label_temp->x_of_center;
	l->y_of_center=label_temp->y_of_center;

	h=add_child(cad,h);
	l->type_of_object=Arc;
	return l;
}
Label *c2d_add_circle(CAD2D *cad){
	Label *l=c2d_add_arc(cad,1,0);
	l->type_of_object=Circle;
	return l;
}
Label *c2d_add_circle_hierarchy(CAD2D *cad,Hierarchy *h){
	Label *l=c2d_add_arc(cad,1,0);
	h=add_child(cad,h);
	l->type_of_object=Circle;
	return l;
}
Label *c2d_add_circle_label(CAD2D *cad,Hierarchy *h,Label *l){
	CAD2D *temp;
	Label *l_temp=c2d_add_arc(cad,0,0);
	temp=cad;
	while(temp->next_object!=NULL){
		temp=temp->next_object;
	}
	//l->cad=temp;
	//temp->label=l;
	h=add_child(cad,h);
	
	l->cad=temp;
	l->x_of_center=l_temp->x_of_center;
	l->y_of_center=l_temp->y_of_center;
	l->type_of_object=Circle;
	return l;
}
void c2d_export(CAD2D *cad,char *file_name,char *options){
	FILE *fp;
	char *str;
	int end_of_file;
	double x_of_point,y_of_point;
	Point2D *temp;
	CAD2D *iter;
	int moveto=0,lineto=1,show=2;
	char arr[100];
	int i=0;
	int counter=0;
	if(cad!=NULL ){
		if(cad->Points_of_objects!=NULL || cad->type_of_object == Text){
		//printf("%s\n",file_name);

		if(!strcmp(options,"eps")){
			//fp=fopen(file_name,"r");
			for(;file_name[i]!='\0';i++){

			}
			i+=3;
			str=(char *)malloc(sizeof(char)*i);
			strcpy(str,file_name);
			strcat(str,".");
			strcat(str,options);
			//printf("%s\n",str );
			//fp=fopen(str,"r");
			/*if(fp!=NULL){
				fscanf(fp,"%s",arr);
				if(arr[0]=='%'){
					printf("AWDWAAFWWFAFWFWAFAW\n");
					//fclose(fp);
					fp=fopen(str,"a");
					fprintf(fp, "%.1lf %lf moveto\n",cad->Points_of_objects->x,cad->Points_of_objects->y );
					iter=cad;
					while(iter!=NULL){
						temp=iter->Points_of_objects;
						while(temp!=NULL){
							fprintf(fp, "%.1lf %.1lf lineto\n", temp->x,temp->y );
							temp=temp->next;
						}
						iter=iter->next_object;
					}
				}
				printf("(AAAAAAAAAA\n");
			}
			else{*/

			fp=fopen(str,"w");

			fprintf(fp,"%%!PS-ADOBE-3.0 EPSF-3.0\n" );
			fprintf(fp,"%%%%BoundingBox: %.0lf %.0lf %.0lf %.0lf \n",-(cad->width)/2,-(cad->height)/2,(cad->width)/2,(cad->height)/2);
			//fprintf(fp,"%%%%BoundingBox: %.0lf %.0lf %.0lf %.0lf \n",-250.0,-250.0,250.0,250.0);
			
			fprintf(fp,"%1.f setlinewidth \n",0.0);
			fprintf(fp, "%d %d %d setrgbcolor \n",0,0,1);
			fprintf(fp, "/Times-Roman findfont\n12 scalefont\nsetfont\nnewpath\n");
			iter=cad;

			while(iter!=NULL && iter->Points_of_objects!=NULL){
			

				if(iter->type_of_object == Text){
					fprintf(fp, "%.1lf %.1lf moveto\n",iter->x_of_center,iter->y_of_center );
					fprintf(fp, "(%s) show\n",iter->text );
				}
				
				else if(iter->Points_of_objects->next!=NULL){
					
						temp=iter->Points_of_objects;
						fprintf(fp, "%.1lf %.1lf moveto\n",iter->Points_of_objects->x,iter->Points_of_objects->y );
						temp=iter->Points_of_objects->next;
						while(temp!=NULL){
							fprintf(fp, "%.1lf %.1lf lineto\n", temp->x,temp->y );
							temp=temp->next;
						}
					
				}
				else {
					// if object is a point
					fprintf(fp, "%.1lf %.1lf moveto\n",iter->Points_of_objects->x,iter->Points_of_objects->y );
					fprintf(fp, "%.1lf %.1lf lineto\n",iter->Points_of_objects->x+0.1,iter->Points_of_objects->y+0.1 );

				}
				iter=iter->next_object;

		}
			fprintf(fp, "%s\n","stroke" );

			fclose(fp);
		}
		if(!strcmp(options,"bin")){
			for(;file_name[i]!='\0';i++){

			}
			i+=3;
			str=(char *)malloc(sizeof(char)*i);
			strcpy(str,file_name);

			strcat(str,".");

			strcat(str,options);

			//printf("%s\n",str);
			fp=fopen(str,"wb");
			//perror("c");

			iter=cad;
			while(iter!=NULL){
				temp=iter->Points_of_objects;
				counter=0;
				while(temp!=NULL){
					if(counter==0){// first we should move to the point
						//printf("MOVETO %.1lf %.1lf\n",temp->x,temp->y);
						fwrite(&moveto,sizeof(int),1,fp);
						fwrite(&(temp->x),sizeof(double),1,fp);
						fwrite(&(temp->y),sizeof(double),1,fp);

						if(iter->type_of_object == Text){// if object is text we should write "('text') show"
							char c='a';
							int len=strlen(iter->text);
							//printf("text uzunlugu: %d\n",len );
							//printf("1. bin calısıyor\n");
							//printf("(%s) SHOW\n",iter->text);

							fwrite(&show,sizeof(int),1,fp);
							fwrite(&len,sizeof(int),1,fp);
							fwrite(iter->text,sizeof(char)*len,1,fp);
						}
						else if(iter->type_of_object == Point && iter->Points_of_objects->next==NULL){
							x_of_point=temp->x+0.1;
							y_of_point=temp->y+0.1;

							fwrite(&lineto,sizeof(int),1,fp);
							fwrite(&x_of_point,sizeof(double),1,fp);
							fwrite(&y_of_point,sizeof(double),1,fp);
						}
					}
					else { // if it is not moveto then it should be lineto
						x_of_point=temp->x;
						y_of_point=temp->y;
						fwrite(&lineto,sizeof(int),1,fp);
						fwrite(&x_of_point,sizeof(double),1,fp);
						fwrite(&y_of_point,sizeof(double),1,fp);
					}

					counter++; // to detect first point
					temp=temp->next;
				}
				iter=iter->next_object;
			}
			end_of_file=-1;//I add it to the end of file to be able to understand there is no object left
			fwrite(&end_of_file,sizeof(int),1,fp);

			fclose(fp);
		}	
}
}
}
void c2d_add_text_import(CAD2D *cad,char *text,double x,double y){
	CAD2D *iter=cad;
	while(iter->next_object!=NULL){
		iter=iter->next_object;
	}
	iter->next_object=(CAD2D*)malloc(sizeof(CAD2D));
	iter=iter->next_object;
	iter->type_of_object=Text;
	iter->x_of_center=x;
	iter->y_of_center=y;
	strcpy(iter->text,text);
	iter->next_object=NULL;
	iter->Points_of_objects=(Point2D*)malloc(sizeof(Point2D));
	iter->Points_of_objects->next=NULL;
}

CAD2D * c2d_import(char *file_name,char *options){
	int i=0;
	int number_of_edges=0;
	int check;//this determines if we have moveto lineto or show
	int d=1;
	char *str;
	int size_of_text;
	CAD2D *cad=NULL;
	CAD2D *temp=NULL;
	Point2D *p=NULL;
	Point2D *iter=NULL;
	CAD2D *unnecessary=NULL;
	Hierarchy *h=c2d_create_hierarchy_as_root(unnecessary);
	//char text[MAX_TEXT_LENGTH];
	char *text=(char*)malloc(sizeof(char)*MAX_TEXT_LENGTH);
	double x,y;

	FILE *fp;
	Label l;
	strcpy(l.name,"Import"); // I will not be able to add Labels unique to the cads because my CAD2D does not contain Label ,Label contains CAD2D

	for(;file_name[i]!='\0';i++){

	}
	i+=3;
	str=(char *)malloc(sizeof(char)*i);
	strcpy(str,file_name);
	strcat(str,".");
	strcat(str,options);
	if(!strcmp(options,"bin")){
		fp=fopen(str,"rb");
		while(1){
			d=fread(&check,sizeof(int),1,fp);
			if(check == -1){ // there is -1 at the end of the file (I added it)
				c2d_add_polyline_label(cad,p,h,&l);
				return cad;
			}
		//	printf("AAA%d\n",check );

			else if(check == 0 || check == 1){
				fread(&x,sizeof(double),1,fp);
				fread(&y,sizeof(double),1,fp);
				
				if(check==0){

					if(cad==NULL){
					//	printf("CAD==NULL\n");
						cad=c2d_start_default();
						temp=cad;
						temp->next_object=NULL;
					}
					if(number_of_edges!=0){ // linked list olustuurp buraya yollıcaz sonra o head e baska yerı malloc etcez oncekı gonderdıgımız yerde kalcak insallah new/3.c ye bak ornegı var :D
					//	printf("yeni obje\n");
						c2d_add_polyline_label(cad,p,h,&l);
						number_of_edges=0;// new object is coming
						p=NULL;
						iter=NULL;
					}
					
					//printf("MOVETO %.1lf %.1lf \n",x,y );
					if(p==NULL){
						//printf("p==NULL\n");
						p=(Point2D*)malloc(sizeof(Point2D));
						p->x=x;
						p->y=y;
						p->next=NULL;
						iter=p;
					}
					
				}
					
					
					/*
					else{
						while(temp->next_object!=NULL){
							temp=temp->next_object;
						}
						temp->next_object=(CAD2D*)malloc(sizeof(CAD2D));
						temp=temp->next_object;
						temp->next_object=NULL;
					}
					*/

				
				else{
					//printf("LİNETO %.1lf %.1lf \n",x,y );
					number_of_edges++;

					while(iter->next!=NULL){
						iter=iter->next;
					}
					iter->next=(Point2D*)malloc(sizeof(Point2D));
					iter=iter->next;
					iter->x=x;
					iter->y=y;
					iter->next=NULL;

				}
			}
			else if(check ==2) {
				fread(&size_of_text,sizeof(int),1,fp);
				//printf("SİZEOFTEXT:%d\n",size_of_text );
				fread(text,sizeof(char)*size_of_text,1,fp);
				text[size_of_text]='\0';
				//printf("(%s) SHOW\n",text );
				c2d_add_text_import(cad,text,x,y);

				//printf("TEXT %.1lf %.1lf ye yzıldı\n", x,y);
				p=NULL;
				number_of_edges=0;

			}
		}
	}
}

Label *c2d_add_ellipse(CAD2D *cad,int this_parameter_should_be_1){
	Label *h=(Label *)malloc(sizeof(Label));
	Point2D *iter=NULL,*head;
	double start_x,end_x,width,height;
	double center_x,center_y;
	double a,b;

	printf("Enter the x value of center of ellipse: ");
	scanf("%lf",&center_x);
	printf("Enter the y value of center of ellipse: ");
	scanf("%lf",&center_y);
	h->x_of_center=center_x;
	h->y_of_center=center_y;
	printf("Enter the vertex of ellipse :");
	scanf("%lf",&width);
	printf("Enter the co-vertex of ellipse :");
	scanf("%lf",&height);
	start_x=-(width)/2;
	end_x=(width)/2;
	a=end_x;
	b=height/2;
	iter=(Point2D*)malloc(sizeof(Point2D));
	head=iter;
	while(start_x<end_x){
		iter->x=start_x+center_x;
		iter->y=sqrt((b*b)-(b*b*(start_x*start_x))/(a*a))+center_y;
		start_x+=(1.0/RESOLUTION);
		iter->next=(Point2D*)malloc(sizeof(Point2D));
		iter=iter->next;
	}
	iter->x=start_x+center_x;
	iter->y=center_y;
	iter->next=(Point2D*)malloc(sizeof(Point2D));
	while(start_x>-width/2){
		iter=iter->next;
		start_x=start_x-(1.0/RESOLUTION);
		iter->x=start_x+center_x;
		iter->y=-sqrt((b*b)-(b*b*(start_x*start_x))/(a*a))+center_y;
		iter->next=(Point2D*)malloc(sizeof(Point2D));
	}
	iter->next=NULL;
	if(this_parameter_should_be_1){//when we implement c2d_add_ellipse_label function I will call this function but this_parameter_should_be_1 will be 0 so I will not take label from user
		strcpy(h->name,take_label("ellipse"));
	}
	add_Point2D_to_given_cad_entity(cad,head,h);
	h->type_of_object=Ellipse;
	return h;
}
Label *c2d_add_ellipse_hierarchy(CAD2D *cad,Hierarchy *h){
	Label *l=c2d_add_ellipse(cad,1);
	h=add_child(cad,h);
	return l;
}
Label *c2d_add_ellipse_label(CAD2D *cad,Hierarchy *h,Label *l){
	CAD2D *temp;
	Label *temp_l=c2d_add_ellipse(cad,0);
	temp=cad;
	while(temp->next_object!=NULL){
		temp=temp->next_object;
	}
	l->cad=temp;
	l->x_of_center=temp_l->x_of_center;
	l->y_of_center=temp_l->y_of_center;
	l->type_of_object=Ellipse;

	h=add_child(cad,h);
	return l;
}
void attach_label_ls_to_closest_point_of_label_lt(Label *ls,Label *lt){ // ls is point lt is polyline
	double distance_between_x= ls->cad->Points_of_objects->x - lt->cad->Points_of_objects->x;
	double distance_between_y= ls->cad->Points_of_objects->y - lt->cad->Points_of_objects->y;
	double min_distance = distance_between_x *distance_between_x + distance_between_y * distance_between_y;
	double temp;
	Point2D *p=lt->cad->Points_of_objects;
	Point2D temp_point;
	temp_point.x=p->x;
	temp_point.y=p->y;

	while(p!=NULL){
		distance_between_x= ls->cad->Points_of_objects->x - p->x;
		distance_between_y= ls->cad->Points_of_objects->y - p->y;
		temp=distance_between_x *distance_between_x + distance_between_y * distance_between_y;
		
		if(temp < min_distance){
			min_distance=temp;
			temp_point.x=p->x;
			temp_point.y=p->y;

		}
		
		p=p->next;

	}

	ls->cad->Points_of_objects->x=temp_point.x;
	ls->cad->Points_of_objects->y=temp_point.y;

	
}
void move_object_to_given_point(Label *ls,Label *lt){

	double differenceX=lt->cad->Points_of_objects->x - ls->cad->Points_of_objects->x;
	double differenceY=lt->cad->Points_of_objects->y - ls->cad->Points_of_objects->y;
	Point2D *p=ls->cad->Points_of_objects;
	// every point of ls will be moved by difference between the first point of object and point

	while(p!=NULL){
		p->x += differenceX;
		p->y += differenceY;
		p=p->next;
	}
}
Label *Label_to_last_point(Label *l){
	Point2D *p=l->cad->Points_of_objects;
	Label *new=(Label *)malloc(sizeof(Label));

	new->cad=(CAD2D *)malloc(sizeof(CAD2D));
	new->cad->Points_of_objects=(Point2D *)malloc(sizeof(Point2D));
	
	while(p->next!=NULL){
		//p will point to last point of Label *l
		p=p->next;
	}
	new->cad->Points_of_objects->x=p->x;
	new->cad->Points_of_objects->y=p->y;
	new->type_of_object=Point; // move_object_to_given_point() function will check it
	return new;

}

/*this function taken from here:
	https://stackoverflow.com/questions/2792443/finding-the-centroid-of-a-polygon 
*/
Point2D *compute2DPolygonCentroid(const Point2D* vertices)
{
    Point2D *centroid =(Point2D*) malloc(sizeof(Point2D));
	
    double signedArea = 0.0;
    double x0 = 0.0; // Current vertex X
    double y0 = 0.0; // Current vertex Y
    double x1 = 0.0; // Next vertex X
    double y1 = 0.0; // Next vertex Y
    double a = 0.0;  // Partial signed area
    centroid->x=0.0;
    centroid->y=0.0;
    // For all vertices except last
    int i=0;
   /*
    for (i=0; i<vertexCount-1; ++i)
    {
        x0 = vertices[i].x;
        y0 = vertices[i].y;
        x1 = vertices[i+1].x;
        y1 = vertices[i+1].y;
        a = x0*y1 - x1*y0;
        signedArea += a;
        centroid->x += (x0 + x1)*a;
        centroid->y += (y0 + y1)*a;
    }
    */
    while(vertices->next!=NULL){
    	//printf("AA %lf %lf AA\n",vertices->x,vertices->y );
        x0=vertices->x;
        y0=vertices->y;
        x1=vertices->next->x;
        y1=vertices->next->y;
        a=x0*y1 - x1*y0;
        signedArea+=a;
        centroid->x+=(x0 + x1) *a;
        centroid->y+=(y0 + y1)*a;
        vertices=vertices->next;
    }
    // Do last vertex separately to avoid performing an expensive
    // modulus operation in each iteration.
    x0 = vertices[i].x;
    y0 = vertices[i].y;
    x1 = vertices[0].x;
    y1 = vertices[0].y;
    a = x0*y1 - x1*y0;
    signedArea += a;
    centroid->x += (x0 + x1)*a;
    centroid->y += (y0 + y1)*a;

    signedArea *= 0.5;
    centroid->x /= (6.0*signedArea);
    centroid->y /= (6.0*signedArea);

    return centroid;
}



void c2d_snap(CAD2D *cad,Label *ls,Label *lt){
	
	/*
	
	Preconditions:	
	
	*   CAD2D *cad should not be NULL
	
	*	Label *ls and Label *lt should hold objects ,that can be realized by assigning this pointers to the return value of c2d_add_... functions
	
	*/

	/*

	Postcondition:

	*	Label *ls snapped to the Label *lt

	*/
	Label *last_point_of_lt;
	double min_distance;
	double distance_between_x,distance_between_y;
	Point2D *p=NULL;
	if(cad==NULL || ls==NULL || lt==NULL || ls->cad==NULL || lt->cad==NULL || ls->cad->Points_of_objects==NULL || lt->cad->Points_of_objects==NULL) 
	{
		printf("Invalid input to the c2d_snap function\n");
		return;
	}

	if (ls->type_of_object == Point) //if ls is pointing to point the next point is NULL
	{
		if(lt->type_of_object == Point)	// If both objects which are about to be snapped are points
		{
			
			ls->cad->Points_of_objects->x=lt->cad->Points_of_objects->x;
			ls->cad->Points_of_objects->y=lt->cad->Points_of_objects->y;
			// ls has been moved to the lt
		}	
	
		else if(lt->type_of_object == Polyline){ // if lt is a polyline
	
			attach_label_ls_to_closest_point_of_label_lt(ls,lt);
	
		}
		else if(lt->type_of_object == Polygon){
			/*
			Point2D *p=lt->cad->Points_of_objects;
			while(p!=NULL){
				printf("f  %lf %lf   f\n",p->x,p->y );
				p=p->next;
			}
			*/
			ls->cad->Points_of_objects=compute2DPolygonCentroid(lt->cad->Points_of_objects); //this function was taken from stackoverflow link in the definition of the function
		}
		else if(lt->type_of_object == Arc){ // lt is arc and the point ls snapped the center of it
	
			ls->cad->Points_of_objects->x=lt->x_of_center;
			ls->cad->Points_of_objects->y=lt->y_of_center;

		}
		else if(lt->type_of_object == Circle || lt->type_of_object == Ellipse){ // lt is a circle or ellipse and ls snapped center of it

			ls->cad->Points_of_objects->x=lt->x_of_center;
			ls->cad->Points_of_objects->y=lt->y_of_center;
	
		}
	}
	else if(ls->type_of_object == Polyline || ls->type_of_object == Polygon){
		
		if(lt->type_of_object == Point){ // polyline ls will be moved to the point lt
		
			move_object_to_given_point(ls,lt);
		
		}
		else if(lt->type_of_object == Polyline || lt->type_of_object==Polygon){
			last_point_of_lt =Label_to_last_point(lt);
			move_object_to_given_point(ls,last_point_of_lt);
		}
	}
	else if(ls->type_of_object == Arc){
		if(lt->type_of_object == Point){
			move_object_to_given_point(ls,lt);
		}
		else if(lt->type_of_object == Polyline || lt->type_of_object == Polygon){
			last_point_of_lt =Label_to_last_point(lt);
			move_object_to_given_point(ls,last_point_of_lt);
		}
	}
}
double Euclidian_distance(Point2D *p1 ,Point2D *p2){
	return sqrt(((p1->x - p2->x)*(p1->x - p2->x)) + ((p1->y - p2->y)*(p1->y - p2->y)));
}
double c2d_measure(CAD2D *cad,const Label *ls,const Label *lt){
	double min_distance=DBL_MAX;
	Point2D *p1;
	Point2D *p2;
	double temp=0;
	double x1,x2,y1,y2;
	int choice=0;

	if(ls!=NULL && lt!= NULL &&ls->cad!=NULL && lt->cad!=NULL){
		printf("Do you want to find distance between centers (0) or shortest between objects (1) :");
		scanf("%d",&choice);
		if(choice==1){
			if(ls->type_of_object == Text || lt->type_of_object == Text){
				printf("Text cannot be used to find euclidian distance");
				return 0;
			}
			else{
				p1=ls->cad->Points_of_objects;
				// very very inefficient way to find distance especially if given objects are circle arc or ellipse , but I could not find any other way

				for(;p1!=NULL;p1=p1->next){
					for(p2=lt->cad->Points_of_objects;p2!=NULL ;p2=p2->next){
						temp=Euclidian_distance(p1,p2);
						//printf("%lf  %lf %lf\n",temp,p2->x,p2->y );
						if(min_distance > temp){
							min_distance=temp;
						}
					}
				}
			}
		}
		else if(choice == 0){
			if(ls->type_of_object ==Polyline || lt->type_of_object == Polyline){
				printf("Center of polyline could not found\n");
				return 0;
			}
			else {
				x1=ls->x_of_center;
				y1=ls->y_of_center;
				x2=lt->x_of_center;
				y2=lt->y_of_center;
				min_distance=sqrt(((x1-x2)*(x1-x2)) +((y1-y2)*(y1-y2)));
			}
		}
	}
	return min_distance;
}
Point2D *take_center(){
	Point2D *p=(Point2D *)malloc(sizeof(Point2D));
	double x,y;
	printf("Enter x value of center : ");
	scanf("%lf",&x);
	printf("Enter y value of center : ");
	scanf("%lf",&y);
	p->x=x;
	p->y=y;
	return p;

}
char *take_text(){
	char *s=(char*)malloc(sizeof(char)*MAX_TEXT_LENGTH);
	char str[MAX_TEXT_LENGTH];
	printf("!!Enter the text: ");
	//scanf("%[^\n]%*c",str);
	//scanf("%s",str);
	scanf(" %[^\n]%*c",str); // I put space in front of it and it started to work properly for inputs which have space character ,I do not know why that is working but anyways

	strcpy(s,str);
	return s;
}
Label *c2d_add_text(CAD2D *cad,int this_parameter_should_be_1,char *THIS_POINTER_SHOULD_BE_NULL,double x,double y){
	Label *l;
	CAD2D *tempCAD2D,*temp;
	Point2D *center_of_text;
	if(cad !=NULL && cad->Points_of_objects ==NULL ){
		l=(Label*)malloc(sizeof(Label));
		cad->Points_of_objects =(Point2D *)malloc(sizeof(Point2D));

		cad->type_of_object = Text;
		cad->next_object=NULL;
		if(THIS_POINTER_SHOULD_BE_NULL == NULL){
			center_of_text=take_center();	

		}

		cad->Points_of_objects->x=center_of_text->x;
		cad->Points_of_objects->y=center_of_text->y;

		l->type_of_object= Text;
		cad->x_of_center =center_of_text->x;
		cad->y_of_center =center_of_text->y;
		/*
		printf("Enter the text (without space): ");
		scanf("%[^\n]%*c",cad->text);
		*/
		if(THIS_POINTER_SHOULD_BE_NULL == NULL){
			printf("POINTER NULL\n");
			strcpy(cad->text,take_text());
		}
		else{
			strcpy(cad->text,THIS_POINTER_SHOULD_BE_NULL);
			cad->x_of_center=x;
			cad->y_of_center=y;
		}
		l->x_of_center=center_of_text->x;
		l->y_of_center=center_of_text->y;
		strcpy(l->text,cad->text);
		if(this_parameter_should_be_1){
			strcpy(l->name,take_label("text"));
		}
		cad->next_object=NULL;
		l->cad=cad;
	}
	
	else if(cad!=NULL){
		l=(Label*)malloc(sizeof(Label));
		tempCAD2D=(CAD2D *)malloc(sizeof(CAD2D));
		tempCAD2D->next_object=NULL;
		tempCAD2D->Points_of_objects =(Point2D *)malloc(sizeof(Point2D));
		if(THIS_POINTER_SHOULD_BE_NULL ==NULL){
			center_of_text=take_center();

		}
		tempCAD2D->Points_of_objects->x=center_of_text->x;
		tempCAD2D->Points_of_objects->y=center_of_text->y;

		
		l->type_of_object= Text;
		tempCAD2D->type_of_object =Text;

		tempCAD2D->x_of_center = center_of_text->x;
		tempCAD2D->y_of_center = center_of_text->y;
		
		//printf("Enter the text (without space): ");
		//scanf("%[^\n]%*c",tempCAD2D->text);
		if(THIS_POINTER_SHOULD_BE_NULL == NULL){
			strcpy(tempCAD2D->text,take_text());
		}
			
		else {
			strcpy(tempCAD2D->text,THIS_POINTER_SHOULD_BE_NULL);
			tempCAD2D->x_of_center=x;
			tempCAD2D->y_of_center=y;
		}
		l->x_of_center=center_of_text->x;
		l->y_of_center=center_of_text->y;
		strcpy(l->text,tempCAD2D->text);
		if(this_parameter_should_be_1){
			strcpy(l->name,take_label("text"));
		}
		temp=cad;
		while(temp->next_object !=NULL){
			temp=temp->next_object;
		}
		temp->next_object=tempCAD2D;
		temp->next_object->next_object=NULL;
		l->cad=tempCAD2D;
	}
	else{
		printf("Please call one of the c2d_start_... function before calling c2d_add_text().\n");
	}
	

	return l;

}
Label *c2d_add_text_hierarchy(CAD2D *cad,Hierarchy *h){
	char *p=NULL;
	Label *l=c2d_add_text(cad,1,p,0,0);
	h=add_child(cad,h);
	return l;
}
Label *c2d_add_text_label(CAD2D *cad,Hierarchy *h,Label *l,char *THIS_POINTER_SHOULD_BE_NULL,double x,double y){
	Label *temp=c2d_add_text(cad,0,THIS_POINTER_SHOULD_BE_NULL,x,y);
	h=add_child(cad,h);
	strcpy(temp->name,l->name);
	return temp;
}