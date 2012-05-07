#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <time.h>

#include <pangolin/pangolin.h>
#include <pangolin/simple_math.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;
using namespace pangolin;

struct CalibParams
{

	vector<string> ImageList;	

	Mat CameraMatrix;
	Mat DistCoeffs;	

};

struct StereoParams
{
	
	Mat R, T, E, F;

};

struct RectifiedParams
{

	Mat LeftRot, RightRot;
	Mat LeftProj, RightProj;
	Rect LeftRoi, RightRoi;
	
	Mat Disp2DepthReProjMat;

	Mat LeftRMAP[2], RightRMAP[2];

	bool isVerticalStereo;
};

class CameraCalibration
{

public:

	CameraCalibration();

	void ReadMonoCalibParams(string &img_xml);

	void ReadStereoCalibParams(string &img_xml);

    void WriteCalibParams();

    void MonoCalibration();

	void StereoCalibration();

	double FundamentalMatrixQuality(vector<vector<Point2f> > LeftImagePoints, vector<vector<Point2f> > RightImagePoints, 
									Mat LeftCameraMatrix, Mat RightCameraMatrix, 
									Mat LeftDistCoeffs, Mat RightDistCoeffs, Mat F);

	void CvtCameraExtrins(const vector<Mat> *RVecs, const vector<Mat> *TVecs);

    void InitPangolin(int PanelWidth);    

    OpenGlMatrixSpec StereoBind(const OpenGlMatrixSpec &LeftCamera);    

	void DrawRectifiedImage(const string &img_file, bool isLeftCamera) const;

	void DrawChessboardAndImage(int c_idx, int img_idx, bool is_undistorted, bool is_stereobind);

	//void OpenCVSBM(const string &left_img, const string &right_img) const;

	CalibParams *calib_params;	
	StereoParams *stereo_params;
	RectifiedParams *rect_params;

    View *panel, *view[2];
	pangolin::GlTexture *gl_img_tex, *gl_chessboard_tex;

	inline void setStereoMode(bool isStereoMode) { stereo_mode = isStereoMode; }

	inline int getNumFrames() const { return NumFrames; }

	inline Size getBoardTexSize() const { return BoardTexSize; }

	// Pangolin matrix
	OpenGlMatrixSpec *CamIntrins;  // Row-major order
	vector<OpenGlMatrixSpec> *CamExtrins;  // Row-major order
	OpenGlMatrixSpec L2RExtrins;	// For stereo mode

private:

	bool stereo_mode;
	string data_path;

	int NumFrames;              // The number of frames to use from the input for calibration
	
	Size BoardSize;            // The size of the board -> Number of items by width and height
	float SquareSize;          // The size of a square in your defined unit (point, millimeter,etc).	
	Size BoardTexSize;	// OpenGL texture for the chessboard rendering
	Size ImageSize;

	// Pangolin OpenGL texture
    void InitTexture();

	void DrawAxis() const;

};
