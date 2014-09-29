#ifndef GEOMETRY_INCLUDED
#define GEOMETRY_INCLUDED

#include <math.h>
#include <stdlib.h>
#include <cfloat>
#include <vadefs.h>
#include <stdarg.h>
#include <fstream>
#include <string.h>
#include <vector>
#include <list>
#include <set>
#include <queue>
#define PI 3.1415926

#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"
using glm::mat4;
using glm::mat3;
using glm::vec3;
using glm::vec4;
template< int dimension>
class Point
{
public:
	Point(void){ for (int i = 0; i < dimension; i++){ coord[i] = 0.f; } }
	Point(double c0, ...);
	double coord[dimension];
	double operator[](int index) const;
	//Point & operator= (const Point & pt);// This is the standard assignment operator
	Point operator+ (const Point & pt) const;
	Point & operator+= (const Point & pt);
	Point operator- (const Point & pt) const;
	Point & operator-= (const Point & pt);
	Point operator* (const double s) const;
	Point operator* (const Point & pt) const;
	Point & operator*= (const double s);
	Point & operator*= (const Point & pt);
	Point operator/ (const double s) const;
	Point operator/ (const Point & pt) const;
	Point & operator/= (const double s);
	Point & operator/= (const Point & pt);
	Point normalvector() const;
	Point & normalize();
	double norm(void) const;
	double squared_norm(void) const;
	double dot(const Point pt) const;
	Point cross(const Point & pt) const;
	Point projection(const Point & pt) const;
	double min_value( int & index) const;
	double min_positive_value(int & index) const;
	double max_value( int & index) const;
};

template<int dimension >
Point<dimension >::Point(double c0, ...)
{
	coord[0] = c0;
	va_list vl;
	va_start(vl, c0);
	for (int i = 1; i<dimension; i++) coord[i] = va_arg(vl, double);
	va_end(vl);
}

template<int dimension>
double Point<dimension>::operator[](int index) const
{
	return coord[index];
}

template<int dimension>
Point<dimension> Point<dimension>::operator+(const Point<dimension> & pt) const
{
	Point<dimension> npoint;
	for (int i = 0; i < dimension; i++){ npoint.coord[i] = coord[i] + pt.coord[i]; }
	return npoint;
}

template<int dimension>
Point<dimension>& Point<dimension>::operator+=(const Point<dimension> & pt)
{
	for (int i = 0; i < dimension; i++){ coord[i] += pt.coord[i]; }
	return *this;
}

template<int dimension>
Point<dimension> Point<dimension>::operator-(const Point<dimension> & pt) const
{
	Point<dimension> npoint;
	for (int i = 0; i < dimension; i++){ npoint.coord[i] = coord[i] - pt.coord[i]; }
	return npoint;
}

template<int dimension>
Point<dimension>& Point<dimension>::operator-=(const Point<dimension> & pt)
{
	for (int i = 0; i < dimension; i++){ coord[i] -= pt.coord[i]; }
	return *this;
}

template<int dimension>
Point<dimension> Point<dimension>::operator*(const double s) const
{
	Point<dimension> npoint;
	for (int i = 0; i < dimension; i++){ npoint.coord[i] = coord[i] * s; }
	return npoint;
}

template<int dimension>
Point<dimension> Point<dimension>::operator*(const Point & pt) const
{
	Point<dimension> npoint;
	for (int i = 0; i < dimension; i++){ npoint.coord[i] = coord[i] * pt.coord[i]; }
	return npoint;
}

template<int dimension>
Point<dimension>& Point<dimension>::operator*=(const double s)
{
	for (int i = 0; i < dimension; i++){ coord[i] *= s; }
	return *this;
}

template<int dimension>
Point<dimension>& Point<dimension>::operator*=(const Point & pt)
{
	for (int i = 0; i < dimension; i++){coord[i] *= pt[i]; }
	return *this;
}

static double sign(double x)
{
	if (x>0.f) return 1.f;
	else if (x<0.f) return -1.f;
	else return 0.f;
}

template<int dimension>
Point<dimension> Point<dimension>::operator/(const double s) const
{
	Point<dimension> npoint;
	if (s == 0.f){
		printf("Zero Division \n");
		for (int i = 0; i < dimension; i++){
			npoint.coord[i] = DBL_MAX*sign(coord[i]);
		}
	}
	else{
		for (int i = 0; i < dimension; i++){ 
			npoint.coord[i] = coord[i] / s; 
		}
	}
	return npoint;
}

template<int dimension>
Point<dimension> Point<dimension>::operator/(const Point & pt) const
{
	Point<dimension> npoint;
	for (int i = 0; i < dimension; i++){
		if (pt.coord[i] == 0.f){ 
			printf("Zero Division \n");
			npoint.coord[i] = DBL_MAX*sign(coord[i]);
		}
		else{
			npoint.coord[i] = coord[i] / pt.coord[i]; 
		}

	}
	return npoint;
}

template<int dimension>
Point<dimension>& Point<dimension>::operator/=(const double s)
{
	if (s == 0.f){
		printf("Zero Division \n"); 
		for (int i = 0; i < dimension; i++){
			coord[i] = DBL_MAX*sign(coord[i]);
		}
	}
	else
	{
		for (int i = 0; i < dimension; i++){
			coord[i] /= s; 
		}
	}
	return *this;
}

template<int dimension>
Point<dimension>& Point<dimension>::operator/=(const Point & pt)
{
	for (int i = 0; i < dimension; i++){
		if (pt.coord[i] == 0.f){
			printf("Zero Division \n"); 
			coord[i] = DBL_MAX*sign(coord[i]);
		}
		else
		{
			coord[i] /= pt[i]; }
		}
		
	return *this;
}


template<int dimension>
Point<dimension>& Point<dimension>::normalize()
{
	if (norm()>0.f)
	{	
		(*this) *= (1.f / norm());
	}
	return *this;
}
template<int dimension>
Point<dimension> Point<dimension>::normalvector() const
{
	Point<dimension> npoint;
	if (norm() > 0.f)
	{
		npoint = (*this)*(1.f / norm());
	}
	return npoint;
}
template<int dimension>
double Point<dimension>::norm(void) const
{
	double d_norm = 0.f;
	for (int i = 0; i < dimension; i++){ d_norm += (coord[i] * coord[i]); }
	d_norm = sqrt(d_norm);
	return d_norm;
}
template<int dimension>
double Point<dimension>::squared_norm(void) const
{
	double d_norm = 0.f;
	for (int i = 0; i < dimension; i++){ d_norm += (coord[i] * coord[i]); }
	return d_norm;
}

template<int dimension>
double Point<dimension>::dot(const Point<dimension> pt) const
{
	double d_dot = 0.f;
	for (int i = 0; i < dimension; i++){ d_dot += (coord[i] * pt.coord[i]); }
	return d_dot;
}

template<int dimension>
Point<dimension> Point<dimension>::cross(const Point<dimension> & pt) const
{
	Point<dimension> npoint;
	if (dimension == 3)
	{
		npoint.coord[0] = coord[1] * pt.coord[2] - coord[2] * pt.coord[1];
		npoint.coord[1] = coord[2] * pt.coord[0] - coord[0] * pt.coord[2];
		npoint.coord[2] = coord[0] * pt.coord[1] - coord[1] * pt.coord[0];
	}
	if (dimension == 2)
	{
		npoint.coord[0] = abs(coord[0] * pt.coord[1] - coord[1] * pt.coord[0]);// Area term
		npoint.coord[1] = 0;
	}
	return npoint;
}

template<int dimension>
double Point<dimension>::min_value( int & index ) const
{
	double temp_min_value = DBL_MAX;
	int temp_index = -1;
	for (int i = 0; i < dimension; i++)
	{
		if (temp_min_value > coord[i])
		{
			temp_min_value = coord[i];
			temp_index = i;
		}
	}
	index = temp_index;
	return temp_min_value;
}

template<int dimension>
double Point<dimension>::min_positive_value(int & index) const
{
	double temp_min_value = DBL_MAX;
	int temp_index = -1;
	for (int i = 0; i < dimension; i++)
	{
		if (temp_min_value > coord[i] && coord[i] > 0.f)
		{
			temp_min_value = coord[i];
			temp_index = i;
		}
	}
	index = temp_index;
	return temp_min_value;
}

template<int dimension>
double Point<dimension>::max_value(int & index) const
{
	double temp_max_value = -DBL_MAX;
	int temp_index = -1;
	for (int i = 0; i < dimension; i++)
	{
		if (temp_max_value < coord[i])
		{
			temp_max_value = coord[i];
			temp_index = i;
		}
	}
	index = temp_index;
	return temp_max_value;
}


template<int dimension>
Point<dimension> Point<dimension>::projection(const Point<dimension> & pt) const
{
	if (this->squared_norm() != 0.f)
	{
		return  (*this)*(this->dot(pt) / this->squared_norm());
	}
	else
	return Point<dimension>();
}

template<int dimension>
Point<dimension> CentralizedRotation(const Point<dimension> & center, const Point<dimension> & direction, const Point<dimension> & position, const double angle)
{
	Point<dimension> centralized_position = position - center;
	Point<dimension> v1 = direction.cross(centralized_position);
	Point<dimension> v2 = direction.cross(v1);
	Point<dimension> rotated_centralized_position = centralized_position + (v1*sin(angle)) + (v2*(1.f-cos(angle)));

	Point<dimension> result = center + rotated_centralized_position;

	return result;
}

// Converters
Point<3> Vec3toPoint3(const vec3 in);
vec3 Point3toVec3(const Point<3> in);

// Matrix 
template< int cols,int rows>
class Matrix{
public:
	double m[cols][rows];
	Matrix(void);
	//Matrix(const Point<3>& p1, const Point<3>& p2, const Point<3>& p3, VectorMode mode);
	
	void InsertColumn(const Point<rows> p, int index);
	void InsertRow(const Point<cols> p, int index);
	static Matrix ColumnMatrix(const Point<rows> p1, ...);
	static Matrix RowMatrix(const Point<cols> p1, ...);
	Point<cols> ExtractColumn(const int index);
	Point<rows> ExtractRow(const int index);
	Point<rows> ReduceColumns();
	Point<cols> ReduceRows();

	/** This method returns the entry of the matrix in the col-th column and the row-th row.*/
	double& operator() (int col, int row);
	/** This method returns the entry of the matrix in the col-th column and the row-th row.*/
	double& index(int col, int row);

	/** This method returns the determinant of the matrix.*/
	double det(void) const;

	template<int ncols, int  nrows> Matrix<ncols, rows> LeftMultiply(const Matrix<ncols, nrows> n);
	template<int ncols, int  nrows> Matrix<cols,  nrows>  RightMultiply(const Matrix<ncols, nrows> n);
	//Point<3> Extract(VectorMode mode,const int index);

	/** This method returns the negative of the matrix */
	Matrix operator- (void) const;

	/** This method multiplies two matrices and returns the product.*/
	Matrix  operator* (const Matrix& m) const;
	/** This method multiplies the current matrix (on the right) by the input matrix..*/
	Matrix& operator*=(const Matrix& m);

	/** This method adds two matrices and returns the sum. */
	Matrix  operator+ (const Matrix& m) const;
	/** This method adds the input matrix to the current matrix. */
	Matrix& operator+=(const Matrix& m);

	/** This method subtracts two matrices and returns the difference. */
	Matrix  operator- (const Matrix& m) const;
	/** This method subtracts the input matrix from the current matrix. */
	Matrix& operator-=(const Matrix& m);

	/** This method scales the entries of a matrix and returns a new matrix. */
	Matrix  operator* (double f) const;
	/** This method scales the entries of the current matrix. */
	Matrix& operator*=(double f);
	/** This method divides the entries of a matrix and returns a new matrix. */
	Matrix  operator/ (double f) const;
	/** This method divides the entries of the current matrix. */
	Matrix& operator/=(double f);

	/** This method returns the transpose of a matrix. (Note that it does not change the entries of the current matrix.)*/
	Matrix<rows,cols> transpose(void) const;
	void transpose(const Matrix<rows, cols> n);
	//static Matrix<rows, cols> Transpose(const Matrix in);
	//template<int ncols, int nrows> Matrix<ncols, nrows> generalTranspose() const;

	/** This method returns the inverse of a matrix. (Note that it does not change the entries of the current matrix.)*/
	Matrix invert(void) const;

	/** This static method tries to invert the input matrix and write it out into the output. A value of 0 is returned if the matrix has 0 determinant.*/
	static int Invert(const Matrix& in, Matrix& out);

	/** This method transforms a 3D point. */
	Point<rows> operator*(const Point<cols>& p) const;

	/** This static method returns the identity matrix. */
	static Matrix IdentityMatrix(void);

	/** This method returns the sum of the squares of the matrix entries */
	double squareNorm(void) const;
	double frobeniusNorm(void) const;
	/** This method returns sum of the squares of the entries of the difference matrix. */
	static double SquareL2Difference(const Matrix& m1, const Matrix& m2);

	/** This method returns the exponent of a matrix */
	static Matrix Exp(const Matrix& m, int iter = 100);
	/** This method returns the logarithm of a matrix */
	static Matrix Log(const Matrix& m, double eps = 0.0001);
	/** This method returns the square-root of a matrix */
	static Matrix SquareRoot(const Matrix& m, double eps = 0.000001);

	/** This method computes the SVD decomposition of the upper 3x3 matrix, such that
	* r1 and r2 are rotations and the upper 3x3 matrix is equal to r1*diagonal*r2 */
	//void SVD(Matrix& r1, Matrix& diagonal, Matrix& r2) const;

	///** This method factors a matrix as the product of a rotation and a symmetric matrix */
	//void Factor(Matrix& rot, Matrix& sym) const;

	///** This method returns the closest 3x3 rotation matrix */
	//Matrix closestRotation(void) const;

	///** This method returns nearest symmetric matrix */
	//Matrix symmetrize(void) const;
	///** This method returns nearest skew-symmetric matrix */
	//Matrix skewSymmetrize(void) const;
};

typedef Matrix<3, 3> Matrix3D;

#include "matrix.inl"

class Image{
public:

	Image(int p_height, int p_width)
	{
		height = p_height;
		width = p_width;
		PixelColors = new Point<4>[height*width];
	}
	Image(int p_height, int p_width, unsigned char * Ucolors);
	~Image(){ delete[] PixelColors; }

	unsigned char * GenerateUnsignedCharPixels(const int channels);
	void WriteImagePPM(const char * file_name);
	void WriteImagePNG(const char * file_name);

	Point<4> * PixelColors;
	int height;
	int width;
	char name[100];
};

Image * ReadImagePNG(const char * file_name);

class Texture{
public:
	Image *img;
	Point<4> BilinearSample(double u, double v);
};

//class SurfaceSample
//{
//public:
//	SurfaceSample(void){
//		color[0] = 1.f;
//		color[1] = 1.f;
//		color[2] = 1.f;
//	}
//
//	Point<3> position;
//	Point<3> normal;
//	Point<3> color;
//	bool visibility[2];
//};
//
//class Visibility;
//
//class ParametricSurface
//{
//public:
//	ParametricSurface(int p_num_samples_side)
//	{
//		num_samples_side = p_num_samples_side;
//		surface_samples = 0;
//	}
//	~ParametricSurface(){ if (surface_samples){ delete[] surface_samples; } }
//	virtual SurfaceSample sample(double u, double v) const = 0;// const = 0 ? -> Inheritance can be const functions!
//	void SetSampleValues(const char * sampling_mode);
//	SurfaceSample * surface_samples;
//	int num_total_samples;
//	int num_samples_side;
//};
//
//class Sphere : public ParametricSurface
//{
//public:
//	Sphere(int p_samples_side, Point<3> p_center, double p_radius, const char* sampling_method) : ParametricSurface(p_samples_side)
//	{
//		center = p_center;
//		radius = p_radius;
//		SetSampleValues(sampling_method);
//	}
//	SurfaceSample sample(double u, double v) const;
//	Point<3> center;
//	double radius;
//};
//
//class ImageSample
//{
//public:
//	ImageSample(void){}
//	Point<2> position;
//	Point<3> color;
//};
//
//class Image{
//public:
//	Image(int p_height, int p_width)
//	{
//		height = p_height;
//		width = p_width;
//		PixelColors = new Point<3>[height*width];
//		//for(int i = 0; i < height; i++){
//		//	for (int j = 0; j < width; j++){
//		//		PixelColors[i*width + j][3] = 1.f; // Point is initially (0,0,0,0)
//		//	}
//		//}
//	}
//	~Image(){delete[] PixelColors; }
//	unsigned char * Image::GenerateUnsignedCharPixels();
//	void WriteImagePPM(const char * file_name);
//	void WriteImagePNG(const char * file_name);
//	Point<3> * PixelColors;
//	int height;
//	int width;
//};
//
//class SampleLocalFrame
//{
//public:
//	SampleLocalFrame(void){}
//	static int Compare(const void* v1, const void* v2);
//	SurfaceSample * sample_ptr;
//	Point<3> plane_reference_coord;
//};
//
//class Visibility
//{
//public:
//
//	Visibility(int p_index, Point<3> p_plane_corner, Point<3> p_plane_up, Point<3> p_plane_right,
//		double p_plane_height, double p_plane_width, int p_height_reCurrent, int p_width_reCurrent, double p_epsilon_tolerance)
//	{
//		index = p_index;
//		plane_corner = p_plane_corner;
//		plane_up = p_plane_up;
//		plane_right = p_plane_right;
//		plane_normal = plane_up.cross(plane_right);
//		plane_height = p_plane_height;
//		plane_width = p_plane_width;
//		height_reCurrent = p_height_reCurrent;
//		width_reCurrent = p_width_reCurrent;
//		epsilon_tolerance = p_epsilon_tolerance;
//		sample_frame = 0;
//		visible_deep = 0;
//	}
//	~Visibility(){if (sample_frame){delete[] sample_frame;} if (visible_deep){delete[] visible_deep;}}
//
//	virtual void SetFrame(int p_num_samples, SurfaceSample * p_samples) = 0;
//	Image GenerateImage(int p_image_heigh, int p_image_width);
//	virtual void SetVisibility(SampleLocalFrame * sample_local_frame) const = 0;
//	virtual Point<3> DirectionToFocus(const SurfaceSample * sample) const = 0;
//
//	int index;
//	Point<3> plane_corner;
//	Point<3> plane_up; // normalized!
//	Point<3> plane_right;// normalized!
//	Point<3> plane_normal;// normalized!
//	Point<3> focus_position;
//	double focus_distance_to_plane;
//	double plane_height;
//	double plane_width;
//	int height_reCurrent;
//	int width_reCurrent;
//	double * visible_deep;
//	double epsilon_tolerance;
//	int num_samples;
//	SampleLocalFrame * sample_frame;
//};
//
//class OrtographicVisibility : public Visibility
//{
//public:
//	OrtographicVisibility(int p_index, Point<3> p_plane_corner, Point<3> p_plane_up, Point<3> p_plane_right,
//		double p_plane_height, double p_plane_width, int p_height_reCurrent, int p_width_reCurrent, double p_epsilon_tolerance) :
//		Visibility(p_index, p_plane_corner, p_plane_up, p_plane_right, p_plane_height, p_plane_width, p_height_reCurrent, p_width_reCurrent, p_epsilon_tolerance)
//	    {
//		focus_distance_to_plane = 0.f;
//		focus_position =(plane_corner + ((plane_up*plane_height + plane_right*plane_width)*0.5f));
//	    }
//
//	void SetFrame(int p_num_samples, SurfaceSample * p_samples);
//	void SetVisibility(SampleLocalFrame * sample_local_frame) const;
//	Point<3> DirectionToFocus(const SurfaceSample * sample) const;
//};
//
//class PerspectiveVisibility : public Visibility
//{
//public:
//	PerspectiveVisibility(int p_index, Point<3> p_plane_corner, Point<3> p_plane_up, Point<3> p_plane_right,
//		double p_plane_height, double p_plane_width, int p_height_reCurrent, int p_width_reCurrent, double p_epsilon_tolerance, double p_focus_distance_to_plane) :
//		Visibility(p_index, p_plane_corner, p_plane_up, p_plane_right, p_plane_height, p_plane_width, p_height_reCurrent, p_width_reCurrent, p_epsilon_tolerance)
//	{
//		focus_distance_to_plane = p_focus_distance_to_plane;
//		focus_position = (plane_corner + ((plane_up*plane_height + plane_right*plane_width)*0.5f)) - plane_normal*focus_distance_to_plane;
//	}
//	void SetFrame(int p_num_samples, SurfaceSample * p_samples);
//	void SetVisibility(SampleLocalFrame * sample_local_frame) const;
//	Point<3> DirectionToFocus(const SurfaceSample * sample) const;
//};
//
//// Scene Definitions
//
//class Light{
//public:
//	Light(Point<3> p_color, Visibility * p_ligth_visbility){
//		color = p_color;
//		ligth_visbility = p_ligth_visbility;
//	}
//	~Light(){ if (ligth_visbility) delete ligth_visbility;}
//	Point<3> color;
//	Visibility *ligth_visbility;
//};
//
//class PointLight : public Light{
//public:
//	PointLight(Point<3> p_color, Visibility * p_ligth_visbility) :Light(p_color, p_ligth_visbility){}
//};
//class DirectionalLight : public Light{
//public:
//	DirectionalLight(Point<3> p_color, Visibility * p_ligth_visbility) :Light(p_color, p_ligth_visbility){}
//};
//
//class ReflectanceModel
//{
//public:
//	ReflectanceModel(void){}
//	virtual double AttenuationFactor(const Point<3> surface_normal, const Point<3> direction_to_light, const Point<3> direction_to_camera) const = 0;
//
//};
//
//class Lambertian : public ReflectanceModel
//{
//public:
//	Lambertian(double p_albedo)
//	{
//		albedo = p_albedo;
//	}
//	double AttenuationFactor(const Point<3> surface_normal, const Point<3> direction_to_light, const Point<3> direction_to_camera) const;
//	double albedo;
//};
//
//class MirrorReflection : public ReflectanceModel
//{
//public:
//	MirrorReflection(double p_albedo, double p_specular_exponent, bool p_add_lambertian_diffusion = true, double p_diffusion_weight = 1.f)
//	{
//		albedo = p_albedo;
//		specular_exponent = p_specular_exponent;
//		add_lambertian_diffusion = p_add_lambertian_diffusion;
//		diffusion_weight = p_diffusion_weight;
//	}
//	double AttenuationFactor(const Point<3> surface_normal, const Point<3> direction_to_light, const Point<3> direction_to_camera) const;
//	double albedo;
//	double specular_exponent;
//	bool add_lambertian_diffusion;
//	double diffusion_weight;
//};
//
//class SpecularFresnelReflection : public ReflectanceModel
//{
//public:
//	SpecularFresnelReflection(double p_albedo, double p_refraction_index, bool p_add_lambertian_diffusion = true, double p_diffusion_weight = 1.f)
//	{
//		albedo = p_albedo;
//		refraction_index = p_refraction_index;
//		add_lambertian_diffusion = p_add_lambertian_diffusion;
//		diffusion_weight = p_diffusion_weight;
//	}
//	double AttenuationFactor(const Point<3> surface_normal, const Point<3> direction_to_light, const Point<3> direction_to_camera) const;
//	double albedo;
//	bool add_lambertian_diffusion;
//	double diffusion_weight;
//	double refraction_index;
//};
//
//
//
//enum DistributionType{Gaussian, Beckmann};
//
//class TorrenceSparrow : public ReflectanceModel
//{
//public:
//	TorrenceSparrow(double p_variance, double p_refraction_index, DistributionType p_distribution, double p_m = 0.2f, bool p_add_lambertian_diffusion = true, double p_albedo = 1.f, double p_diffusion_weight = 1.f)
//	{
//		variance = p_variance;
//		inv_sqrt_2_pi_variance = 1.f / sqrt(2.f*3.14159265f*variance);
//		refraction_index = p_refraction_index;
//		distribution = p_distribution;
//		m = p_m;
//		add_lambertian_diffusion = p_add_lambertian_diffusion;
//		albedo = p_albedo;
//		diffusion_weight = p_diffusion_weight;
//	}
//	double AttenuationFactor(const Point<3> surface_normal, const Point<3> direction_to_light, const Point<3> direction_to_camera) const;
//	double variance;
//	double refraction_index;
//	double inv_sqrt_2_pi_variance;
//	DistributionType distribution;
//	double m;
//	double albedo;
//	bool add_lambertian_diffusion;
//	double diffusion_weight;
//};
//
//class OrenNayar : public ReflectanceModel
//{
//public:
//	OrenNayar(double p_variance, double p_albedo)
//	{
//		variance = p_variance;
//		A = 1.f - 0.5*(variance / (variance + 0.33f));
//		B = 0.45f*(variance / (variance + 0.09f));
//		albedo = p_albedo;
//	}
//	double AttenuationFactor(const Point<3> surface_normal, const Point<3> direction_to_light, const Point<3> direction_to_camera) const;
//	double variance;
//	double A;
//	double B;
//	double albedo;
//};
//
//class DiffuseReflection : public ReflectanceModel
//{
//public:
//	DiffuseReflection(double p_refraction_index, double p_albedo)
//	{
//		albedo = p_albedo;
//		refraction_index=p_refraction_index;
//	}
//	double AttenuationFactor(const Point<3> surface_normal, const Point<3> direction_to_light, const Point<3> direction_to_camera) const;
//	double refraction_index;
//	double albedo;
//};
//
//class Scene
//{
//public:
//	Scene(Light * p_light, Visibility * p_camera, ParametricSurface *p_surface, ReflectanceModel * p_ref_model)
//	{
//		light = p_light;
//		camera = p_camera;
//		surface = p_surface;
//		ref_model = p_ref_model;
//	}
//	Image * render(int p_image_height, int p_image_width);
//	Light * light;
//	Visibility * camera;
//	ParametricSurface * surface;
//	ReflectanceModel * ref_model;
//};
//
//double SimpleFresnelRefelectionFactor(double n, double cos_angle_mid);
//double ComplexFresnelRefelectionFactor(double n, double k, double cos_angle);

#endif // GEOMETRY_INCLUDED