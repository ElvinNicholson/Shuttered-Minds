#include "LightSource.h"

LightSource::LightSource(std::shared_ptr<sf::Vector2f> _start, sf::Color _color):
start(_start), color(_color)
{
  last_position = sf::Vector2f(0, 0);
  myRenderTexture.create(1080, 720);

  visible = true;
  radius = 4;
}

void LightSource::update(float dt, const std::vector<Edge>& vector_edges)
{
  if (last_position != *start)
  {
    // Update rays only when player moves
    calculateRayCollisions(*start, vector_edges);
    createTriangles();
  }
  last_position = *start;
}

void LightSource::render(sf::RenderTexture& render_tex, const sf::View& view, sf::Shader& shader)
{
  if (!visible)
  {
    return;
  }

  sf::Vector2f lightPos = sf::Vector2f(render_tex.mapCoordsToPixel(*start, view));
  shader.setUniform("lightPos", lightPos);
  shader.setUniform("color", sf::Glsl::Vec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1));
  shader.setUniform("radius", radius);

  myRenderTexture.clear();
  myRenderTexture.setView(view);

  for (auto& triangle : triangles)
  {
    myRenderTexture.draw(triangle, &shader);
  }

  myRenderTexture.display();

  render_tex.draw(sf::Sprite(myRenderTexture.getTexture()), sf::BlendAdd);
}

/// Calculates the collision points of the light rays
/// @param [in] origin The origin of the light source
/// @param [in] vector_edges A constant reference to the vector_edges
void LightSource::calculateRayCollisions(sf::Vector2f origin, const std::vector<Edge>& vector_edges)
{
  // Clear previous collision points
  collision_points.clear();

  // Loop through each edges
  for (auto& edge_1 : vector_edges)
  {
    // Cast 2 rays to start and end of edge
    for (int i = 0; i < 2; i++)
    {
      sf::Vector2f gradient ((i == 0 ? edge_1.start.x : edge_1.end.x) - origin.x,
                            (i == 0 ? edge_1.start.y : edge_1.end.y) - origin.y);

      float base_angle = atan2f(gradient.y, gradient.x);

      float angle = 0;

      // Cast 3 more rays with small offset
      // 1st ray is slightly above the edge point
      // 2nd ray is exactly towards the edge point
      // 3rd ray is slightly below the edge point
      for (int j = -1; j < 2; j ++)
      {
        angle = base_angle + j * 0.0001f;

        gradient = sf::Vector2f (cosf(angle),
                                sinf(angle));

        float min_t1 = INFINITY;
        // min_point.z is angle
        sf::Vector3f min_point = sf::Vector3f (0, 0, 0);
        bool hit = false;

        // Checking for intersection
        for (auto& edge_2 : vector_edges)
        {
          sf::Vector2f segment = sf::Vector2f (edge_2.end.x - edge_2.start.x,
                                              edge_2.end.y - edge_2.start.y);

          // Check if lines are parallel
          if (fabs(segment.x - gradient.x) > 0 &&
              fabs(segment.y - gradient.y) > 0)
          {
            // Distance along edge
            float t2 = (gradient.x * (edge_2.start.y - origin.y) + (gradient.y * (origin.x - edge_2.start.x))) /
                       (segment.x * gradient.y - segment.y * gradient.x);

            // Distance along ray
            float t1 = (edge_2.start.x + segment.x * t2 - origin.x) / gradient.x;

            if (t1 > 0 &&
                t2 >= 0 &&
                t2 <= 1)
            {
              if (t1 < min_t1)
              {
                min_t1 = t1;
                min_point.x = origin.x + gradient.x * t1;
                min_point.y = origin.y + gradient.y * t1;
                min_point.z = atan2f(min_point.y - origin.y, min_point.x - origin.x);
                hit = true;
              }
            }
          }
        }

        // Add collision points to vector of collision points
        if (hit)
        {
          collision_points.emplace_back(min_point);
        }
      }
    }
  }

  // Sort the collision points so the triangles are made in a clockwise direction
  std::sort(collision_points.begin(),
            collision_points.end(),
            [&]( const sf::Vector3f& v1, const sf::Vector3f& v2 )
            {
              return (v1.z < v2.z);
            });
}

/// Creates the triangles that represent the shape of the light that will be rendered
void LightSource::createTriangles()
{
  // Remove points that are too close to each other
  auto it = std::unique(collision_points.begin(),
                        collision_points.end(),
                        [&]( const sf::Vector3f& v1, const sf::Vector3f& v2 )
                        {
                          return fabs(v1.x - v2.x) < 0.1f && fabs(v1.y - v2.y) < 0.1f;
                        });

  collision_points.resize(std::distance(collision_points.begin(), it));

  triangles.clear();

  // Loop through the vector of collision points to create the triangles in a clockwise direction
  for (int i = 0; i < collision_points.size() - 1; i++)
  {
    sf::ConvexShape new_triangle;
    new_triangle.setPointCount(3);

    new_triangle.setPoint(0, *start);
    new_triangle.setPoint(1, sf::Vector2f(collision_points[i].x, collision_points[i].y));
    new_triangle.setPoint(2, sf::Vector2f(collision_points[i + 1].x, collision_points[i + 1].y));

    new_triangle.setFillColor(color);

    triangles.emplace_back(new_triangle);
  }

  // Complete the shape by linking the final collision point with the first
  sf::ConvexShape new_triangle;
  new_triangle.setPointCount(3);

  new_triangle.setPoint(0, *start);
  new_triangle.setPoint(1, sf::Vector2f(collision_points[collision_points.size() - 1].x, collision_points[collision_points.size() - 1].y));
  new_triangle.setPoint(2, sf::Vector2f(collision_points[0].x, collision_points[0].y));

  new_triangle.setFillColor(color);

  triangles.emplace_back(new_triangle);
}
