#ifndef myHeader
#define myHeader
#define CANVAS_SIZE_without_parameter 500
#define MAX_LABEL_LENGTH 10
#define RESOLUTION 10.0
#define MAX_TEXT_LENGTH 50

enum Objects{Point ,Polyline,Polygon,Arc,Circle,Ellipse,Text};

typedef struct Point2D
{
	double x,y;
	struct Point2D *next;
}Point2D;

typedef struct CAD2D{//CAD2D is linked list.First node of this list contains width and height of its canvas
	double width,height;
	Point2D *Points_of_objects;//head of linked list of points of object
	double x_of_center,y_of_center;
	char text[MAX_TEXT_LENGTH];
	int type_of_object;
	struct CAD2D *next_object;
}CAD2D;
typedef struct Label{
	char name[MAX_LABEL_LENGTH];
	char text[MAX_TEXT_LENGTH];
	// only rule: Names' lenght should be less than MAX_LABEL_LENGTH
	//we'll check name of labels to be able to make them unique 
	CAD2D *cad;
	double x_of_center,y_of_center;
	int type_of_object;
}Label;

typedef struct Hierarchy{
	CAD2D *object;//Head of linked list
	struct Hierarchy *child;
	struct Hierarchy *sibling;
}Hierarchy;

CAD2D *c2d_start_default();
CAD2D *c2d_start_with_parameters(double width,double height);
CAD2D *c2d_start_to_the_hierarchy(double width,double height, Hierarchy *h);

Label* c2d_add_point_xy(CAD2D *cad,double x,double y);
Label* c2d_add_point_point(CAD2D *cad,Point2D *p);
Label* c2d_add_point_hierarchy(CAD2D *cad,Point2D *p,Hierarchy *h);
Label* c2d_add_pointlabel(CAD2D *cad,Point2D *p,Hierarchy *h,Label * l);

Hierarchy *c2d_create_hierarchy_as_root(CAD2D *cad);
Hierarchy *c2d_create_hierarchy_as_child(CAD2D *cad,Hierarchy *parent);

Label *c2d_add_polyline_xy(CAD2D *cad,double points[][2],int number_of_edges);
Point2D *create_linkedlist_of_points(double arr[][2], int size);

Label *c2d_add_polyline_point(CAD2D* cad,Point2D *p);//p should be head of linked list which contains points
Label *c2d_add_polyline_hierarchy(CAD2D *cad,Point2D *p,Hierarchy *h);
Label *c2d_add_polyline_label(CAD2D *cad,Point2D *p,Hierarchy *h,Label *l);

Label *c2d_add_polygon_xy(CAD2D *cad,double points[][2],int number_of_edges);
Label *c2d_add_polygon_point(CAD2D* cad,Point2D *p);//p should be head of linked list which contains points
Label *c2d_add_polygon_hierarchy(CAD2D *cad,Point2D *p,Hierarchy *h);
Label *c2d_add_polygon_label(CAD2D *cad,Point2D *p,Hierarchy *h,Label *l);

Label *c2d_add_arc(CAD2D *cad,int this_parameter_should_be_1,int this_parameter_should_be_1_again);//properties of arc will be wanted in the funciton
Label *c2d_add_arc_hierarchy(CAD2D *cad,Hierarchy *h);
Label *c2d_add_arc_label(CAD2D *cad,Hierarchy *h,Label *l);

Label *c2d_add_circle(CAD2D *cad);
Label *c2d_add_circle_hierarchy(CAD2D *cad,Hierarchy *h);
Label *c2d_add_circle_label(CAD2D *cad,Hierarchy *h,Label *l);

Label *c2d_add_ellipse(CAD2D *cad,int this_parameter_should_be_1);
Label *c2d_add_ellipse_hierarchy(CAD2D *cad,Hierarchy *h);
Label *c2d_add_ellipse_label(CAD2D *cad,Hierarchy *h,Label *l);

Label *c2d_add_text(CAD2D *cad,int this_parameter_should_be_1,char *THIS_POINTER_SHOULD_BE_NULL,double x,double y);//x and y does not matter It is for me if user put some value it does not change anything if THIS_POINTER_SHOULD_BE_NULL is NULL

Label *c2d_add_text_hierarchy(CAD2D *cad,Hierarchy *h);
Label *c2d_add_text_label(CAD2D *cad,Hierarchy *h,Label *l,char * THIS_POINTER_SHOULD_BE_NULL,double x,double y);
void c2d_snap(CAD2D *cad,Label *ls,Label *lt);

void c2d_export(CAD2D *cad,char *file_name,char *options);
CAD2D * c2d_import(char *file_name,char *options);
double c2d_measure(CAD2D *cad,const Label *ls,const Label *lt);

Point2D *compute2DPolygonCentroid(const Point2D* vertices);
Hierarchy* add_child(CAD2D *cad,Hierarchy *head);
Point2D *create_arc_between_given_angles(double x_of_center,double y_of_center,double diameter,double start_angle,double end_angle);
void print_Tree(Hierarchy *head);
void add_hie(Hierarchy *h);
void print_value_of_hie(Hierarchy *h);
void my_free(Hierarchy *root);
void print_points(CAD2D *cad);
void print_point(CAD2D *cad);
void free_point(Point2D *temp);
void print_text(Hierarchy *h);
int make_sure_label_is_unique(char *s);
void add_Point2D_to_given_cad_entity(CAD2D *cad,Point2D* points, Label *label);
void c2d_add_text_import(CAD2D *cad,char *text,double x,double y);
void attach_label_ls_to_closest_point_of_label_lt(Label *ls,Label *lt);
void move_object_to_given_point(Label *ls,Label *lt);
Label *Label_to_last_point(Label *l);
double Euclidian_distance(Point2D *p1 ,Point2D *p2);
Point2D *take_center();
char *take_text();

#endif