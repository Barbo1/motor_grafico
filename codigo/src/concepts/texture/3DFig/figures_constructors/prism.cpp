#include "../../../../../headers/concepts/texture.hpp"
#include <cstdlib>

Visualizer<D3FIG> Visualizer<D3FIG>::prism (SDL_Renderer* render, float base, float height, float depth) {
  Visualizer<D3FIG> fig = Visualizer<D3FIG>();

  fig.info->vectors.reserve(8);
  fig.info->vectors.emplace_back(-base, -height, depth);
  fig.info->vectors.emplace_back(-base, height, depth);
  fig.info->vectors.emplace_back(-base, -height, -depth);
  fig.info->vectors.emplace_back(-base, height, -depth);
  fig.info->vectors.emplace_back(base, -height, depth);
  fig.info->vectors.emplace_back(base, height, depth);
  fig.info->vectors.emplace_back(base, -height, -depth);
  fig.info->vectors.emplace_back(base, height, -depth);

  fig.info->normals.reserve(6);
  fig.info->normals.emplace_back(-1.0000, 0.0000, 0.0000);
  fig.info->normals.emplace_back(0.0000, 0.0000, -1.0000);
  fig.info->normals.emplace_back(1.0000, 0.0000, 0.0000 );
  fig.info->normals.emplace_back(0.0000, 0.0000, 1.0000 );
  fig.info->normals.emplace_back(0.0000, -1.0000, 0.0000);
  fig.info->normals.emplace_back(0.0000, 1.0000, 0.0000 );

  fig.info->mapping.reserve(14);
  fig.info->mapping.emplace_back(1.000000, 0.333333);
  fig.info->mapping.emplace_back(1.000000, 0.666666);
  fig.info->mapping.emplace_back(0.750000, 0.333333);
  fig.info->mapping.emplace_back(0.500000, 0.666666);
  fig.info->mapping.emplace_back(0.500000, 0.333333);
  fig.info->mapping.emplace_back(0.250000, 0.666666);
  fig.info->mapping.emplace_back(0.250000, 0.333333);
  fig.info->mapping.emplace_back(0.000000, 0.666666);
  fig.info->mapping.emplace_back(0.000000, 0.333333);
  fig.info->mapping.emplace_back(0.500000, 0.000000);
  fig.info->mapping.emplace_back(0.250000, 0.000000);
  fig.info->mapping.emplace_back(0.500000, 1.000000);
  fig.info->mapping.emplace_back(0.750000, 0.666666);
  fig.info->mapping.emplace_back(0.250000, 1.000000);
  
  fig.info->indeces.reserve(12);
  fig.info->indeces.emplace_back(Face {0, {0,1,2}, {0,1,2}});
  fig.info->indeces.emplace_back(Face {1, {2,7,6}, {2,3,4}});
  fig.info->indeces.emplace_back(Face {2, {6,5,4}, {4,5,6}});
  fig.info->indeces.emplace_back(Face {3, {4,1,0}, {6,7,8}});
  fig.info->indeces.emplace_back(Face {4, {2,6,0}, {9,4,10}});
  fig.info->indeces.emplace_back(Face {5, {7,3,5}, {3,11,5}});
  fig.info->indeces.emplace_back(Face {0, {2,1,3}, {2,1,12}});
  fig.info->indeces.emplace_back(Face {3, {1,4,5}, {7,6,5}});
  fig.info->indeces.emplace_back(Face {2, {5,6,7}, {5,4,3}});
  fig.info->indeces.emplace_back(Face {1, {7,2,3}, {3,2,12}});
  fig.info->indeces.emplace_back(Face {5, {5,3,1}, {5,11,13}});
  fig.info->indeces.emplace_back(Face {4, {0,6,4}, {10,4,6}});

  return fig;
}
