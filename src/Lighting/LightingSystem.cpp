#include "LightingSystem.h"

/// Constructor of the LightingSystem
/// @param [in] width The width of the tilemap
/// @param [in] height The height of the tilemap
/// @param [in] _tile_size The dimensions of each tiles in pixels
LightingSystem::LightingSystem(int width, int height, int _tile_size): 
  tile_size(_tile_size)
{
  // Shader
  shader.loadFromFile("Data/Shaders/Lighting.frag", sf::Shader::Fragment);

  // Edge Map
  for (int x = 0; x < width; x += 1)
  {
    obstacle_map.emplace_back();
    for (int y = 0; y < height; y += 1)
    {
      obstacle_map[x].emplace_back(std::make_unique<Tile>());
    }
  }
}

void LightingSystem::update(float dt)
{
  for (auto& light : light_sources)
  {
    light->update(dt, vector_edges);
  }
}

void LightingSystem::render(sf::RenderTexture& render_tex, const sf::View& view)
{
  for (auto& light : light_sources)
  {
    light->render(render_tex, view, shader);
  }

  // Debug for tileMap edges
//  for (auto& line : edge_lines)
//  {
//    render_tex.draw(line.vertex, 2, sf::Lines);
//  }
//  for (auto& dot : edge_dots)
//  {
//    render_tex.draw(dot);
//  }
}

/// Changes the visibility of the light source at the index
/// @param [in] visibility TRUE = ON, FALSE = OFF
/// @param [in] index The index of the light source in the light_sources vector
void LightingSystem::setLightVisible(bool visibility, int index)
{
  light_sources.at(index)->setVisible(visibility);
}

/// Changes the radius of the light source at the index
/// @param [in] visibility Higher value = smaller, lower value = bigger
/// @param [in] index The index of the light source in the light_sources vector
void LightingSystem::setLightRadius(float radius, int index)
{
  light_sources.at(index)->setRadius(radius);
}

/// Sets the tile at the given position as an obstacle
/// @param [in] x X-Coordinate of the obstacle
/// @param [in] y Y-Coordinate of the obstacle
void LightingSystem::setObstacle(int x, int y)
{
  obstacle_map[x][y]->is_obstacle = true;
}

/// Sets the tile at the given position as bar
/// @param [in] barPos The position of the bar
void LightingSystem::createBar(sf::Vector2i barPos)
{
  vector_edges.emplace_back();
  vector_edges.back().start = sf::Vector2f (barPos.x * tile_size + 2, barPos.y * tile_size + 42);
  vector_edges.back().end = sf::Vector2f (barPos.x * tile_size + 6, barPos.y * tile_size + 42);

  vector_edges.emplace_back();
  vector_edges.back().start = sf::Vector2f (barPos.x * tile_size + 14, barPos.y * tile_size + 42);
  vector_edges.back().end = sf::Vector2f (barPos.x * tile_size + 20, barPos.y * tile_size + 42);

  vector_edges.emplace_back();
  vector_edges.back().start = sf::Vector2f (barPos.x * tile_size + 26, barPos.y * tile_size + 42);
  vector_edges.back().end = sf::Vector2f (barPos.x * tile_size + 32, barPos.y * tile_size + 42);

  vector_edges.emplace_back();
  vector_edges.back().start = sf::Vector2f (barPos.x * tile_size + 38, barPos.y * tile_size + 42);
  vector_edges.back().end = sf::Vector2f (barPos.x * tile_size + 44, barPos.y * tile_size + 42);
}

/// Creates lines that visually represents the vector of edges, used for debugging
void LightingSystem::createDebugLines()
{
  for (auto& edge : vector_edges)
  {
    edge_lines.emplace_back(Line());
    edge_lines.back().vertex[0].position = edge.start;

    edge_lines.back().vertex[1].position = edge.end;

    edge_dots.emplace_back(sf::CircleShape(3));
    edge_dots.back().setPosition(edge.start - sf::Vector2f(3, 3));
    edge_dots.back().setFillColor(sf::Color::Red);

    edge_dots.emplace_back(sf::CircleShape(3));
    edge_dots.back().setPosition(edge.end - sf::Vector2f(3, 3));
    edge_dots.back().setFillColor(sf::Color::Red);
  }
}

/// Creates a light source
/// @param [in] position The position of the light source
/// @param [in] color The color of the light source
void LightingSystem::createLightSource(std::shared_ptr<sf::Vector2f> position, sf::Color color)
{
  light_sources.emplace_back(std::make_unique<LightSource>(position, color));
}

/// Converts the obstacle map to a vector of edges
void LightingSystem::createEdgeMap()
{
  vector_edges.clear();

  // Loop through the obstacle map
  for (int y = 0; y < obstacle_map[0].size(); y++)
  {
    for (int x = 0; x < obstacle_map.size(); x++)
    {
      // Check if tile is an obstacle
      if (obstacle_map[x][y]->is_obstacle)
      {
        // Check if WEST neighbour is an obstacle
        if (x - 1 >= 0 &&
            !obstacle_map[x - 1][y]->is_obstacle)
        {
          // Has no WEST neighbour, need to add WEST edge
          // Check if NORTH neighbour has a WEST edge
          if (y - 1 >= 0 && obstacle_map[x][y - 1]->edge_exist[WEST])
          {
            // NORTH neighbour has a WEST edge
            vector_edges[obstacle_map[x][y - 1]->edge_id[WEST]].end.y += tile_size;
            obstacle_map[x][y]->edge_id[WEST] =
              obstacle_map[x][y - 1]->edge_id[WEST];
            obstacle_map[x][y]->edge_exist[WEST] = true;
          }
          else
          {
            // Has no NORTH neighbour, create an edge
            Edge edge;
            edge.start = sf::Vector2f (x * tile_size, y * tile_size);
            edge.end = sf::Vector2f (edge.start.x, edge.start.y + tile_size);

            int edge_id = vector_edges.size();
            vector_edges.emplace_back(edge);

            obstacle_map[x][y]->edge_id[WEST] = edge_id;
            obstacle_map[x][y]->edge_exist[WEST] = true;
          }
        }

        // Check if EAST neighbour is an obstacle
        if (x + 1 < obstacle_map.size() &&
            !obstacle_map[x + 1][y]->is_obstacle)
        {
          // Has no EAST neighbour, need to add EAST edge
          // Check if NORTH neighbour has a EAST edge
          if (y - 1 >= 0 && obstacle_map[x][y - 1]->edge_exist[EAST])
          {
            // NORTH neighbour has a EAST edge
            vector_edges[obstacle_map[x][y - 1]->edge_id[EAST]].end.y += tile_size;
            obstacle_map[x][y]->edge_id[EAST] =
              obstacle_map[x][y - 1]->edge_id[EAST];
            obstacle_map[x][y]->edge_exist[EAST] = true;
          }
          else
          {
            // Has no NORTH neighbour, create an edge
            Edge edge;
            edge.start = sf::Vector2f ((x + 1) * tile_size, y * tile_size);
            edge.end = sf::Vector2f (edge.start.x, edge.start.y + tile_size);

            int edge_id = vector_edges.size();
            vector_edges.emplace_back(edge);

            obstacle_map[x][y]->edge_id[EAST] = edge_id;
            obstacle_map[x][y]->edge_exist[EAST] = true;
          }
        }

        // Check if NORTH neighbour is an obstacle
        if (y - 1 >= 0 &&
            !obstacle_map[x][y - 1]->is_obstacle)
        {
          // Has no NORTH neighbour, need to add NORTH edge
          // Check if WEST neighbour has a NORTH edge
          if (x - 1 >= 0 && obstacle_map[x - 1][y]->edge_exist[NORTH])
          {
            // WEST neighbour has a NORTH edge
            vector_edges[obstacle_map[x - 1][y]->edge_id[NORTH]].end.x += tile_size;
            obstacle_map[x][y]->edge_id[NORTH] =
              obstacle_map[x - 1][y]->edge_id[NORTH];
            obstacle_map[x][y]->edge_exist[NORTH] = true;
          }
          else
          {
            // Has no WEST neighbour, create an edge
            Edge edge;
            edge.start = sf::Vector2f (x * tile_size, y * tile_size);
            edge.end = sf::Vector2f (edge.start.x + tile_size, edge.start.y);

            int edge_id = vector_edges.size();
            vector_edges.emplace_back(edge);

            obstacle_map[x][y]->edge_id[NORTH] = edge_id;
            obstacle_map[x][y]->edge_exist[NORTH] = true;
          }
        }

        // Check if SOUTH neighbour is an obstacle
        if (y + 1 < obstacle_map[0].size() &&
            !obstacle_map[x][y + 1]->is_obstacle)
        {
          // Has no SOUTH neighbour, need to add SOUTH edge
          // Check if WEST neighbour has a SOUTH edge
          if (x - 1 >= 0 && obstacle_map[x - 1][y]->edge_exist[SOUTH])
          {
            // WEST neighbour has a SOUTH edge
            vector_edges[obstacle_map[x - 1][y]->edge_id[SOUTH]].end.x += tile_size;
            obstacle_map[x][y]->edge_id[SOUTH] =
              obstacle_map[x - 1][y]->edge_id[SOUTH];
            obstacle_map[x][y]->edge_exist[SOUTH] = true;
          }
          else
          {
            // Has no SOUTH neighbour, create an edge
            Edge edge;
            edge.start = sf::Vector2f (x * tile_size, (y + 1) * tile_size);
            edge.end = sf::Vector2f (edge.start.x + tile_size, edge.start.y);

            int edge_id = vector_edges.size();
            vector_edges.emplace_back(edge);

            obstacle_map[x][y]->edge_id[SOUTH] = edge_id;
            obstacle_map[x][y]->edge_exist[SOUTH] = true;
          }
        }
      }
    }
  }

  // Clear obstacle_map since it is no longer used
  obstacle_map.clear();
}
