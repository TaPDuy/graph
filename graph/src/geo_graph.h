#pragma once
#include <string>
#include <vector>

#ifndef GEO_GRAPH
#define GEO_GRAPH

class  GeoGraph {
private:
	size_t length;
	int subPlotNumber;
	double* depth;

	class GeoGrapghAttribute {
	public:
		std::string name;
		double* value;
		int subplotIndex;
		bool filled;
		double shade_line;
		/*double minAxisLimit;
		double maxAxisLimit;
		double minZoom;
		double maxZoom;*/

		GeoGrapghAttribute(
			std::string name,
			double* value,
			int subplotIndex,
			bool filled = false,
			double shade_line = 0
			// double minAxisLimit,
			// double maxAxisLimit,
			// double minZoom,
			// double maxZoom
		);
	};
	std::vector<GeoGrapghAttribute> attributes;

	void drawSubGraph(int index);

public:
	GeoGraph(size_t length, int subPlotNumber, double* depth);

	void addAttribute(
		std::string name,
		double* value,
		int subplotIndex = 0,
		bool filled = false,
		double shade_line = 0
		/*double minAxisLimit,
		double maxAxisLimit,
		double minZoom,
		double maxZoom*/
	);

	void draw();

};

#endif // !GEO_GRAPH
