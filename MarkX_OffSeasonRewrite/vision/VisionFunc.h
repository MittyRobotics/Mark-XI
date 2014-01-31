//Last edited by Vadim Korolik
//on 11/07/2013
#include "WPILib.h"
#include "Vision/RGBImage.h"
#include "Vision/BinaryImage.h"
#include "Math.h"
#include "../Definitions.h"
#include "../log/TKOLogger.h"
 
/**
 * Sample program to use NIVision to find rectangles in the scene that are illuminated
 * by a LED ring light (similar to the model from FIRSTChoice). The camera sensitivity
 * is set very low so as to only show light sources and remove any distracting parts
 * of the image.
 * 
 * The CriteriaCollection is the set of criteria that is used to filter the set of
 * rectangles that are detected. In this example we're looking for rectangles with
 * a minimum width of 30 pixels and maximum of 400 pixels.
 * 
 * The algorithm first does a color threshold operation that only takes objects in the
 * scene that have a bright green color component. Then a convex hull operation fills 
 * all the rectangle outlines (even the partially occluded ones). Then a small object filter
 * removes small particles that might be caused by green reflection scattered from other 
 * parts of the scene. Finally all particles are scored on rectangularity, aspect ratio,
 * and hollowness to determine if they match the target.
 *
 * Look in the VisionImages directory inside the project that is created for the sample
 * images as well as the NI Vision Assistant file that contains the vision command
 * chain (open it with the Vision Assistant)
 */

class VisionFunc
{	
	public:
		static VisionFunc* inst();
		double VisionFunc::computeDistance (BinaryImage *image, ParticleAnalysisReport *report, bool outer);
		double VisionFunc::scoreAspectRatio(BinaryImage *image, ParticleAnalysisReport *report, bool outer);
		bool VisionFunc::scoreCompare(Scores scores, bool outer);
		double VisionFunc::scoreRectangularity(ParticleAnalysisReport *report);
		double VisionFunc::scoreXEdge(BinaryImage *image, ParticleAnalysisReport *report);
		double VisionFunc::scoreYEdge(BinaryImage *image, ParticleAnalysisReport *report);
		
	private:
		VisionFunc();
		~VisionFunc();
		static VisionFunc* m_Instance;
};

