#ifndef TEXTURE_ATLAS_HPP
#define TEXTURE_ATLAS_HPP

#include <string>
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

struct SpriteInfo {
    glm::vec2 top_left;
    glm::vec2 bottom_right;
};

/*struct RenderData {*/
/*    std::vector<float> uv_coords;*/
/*    unsigned int texture_name;*/
/*};*/

class TextureAtlas {
  public:
    TextureAtlas(const std::string &json_path, const std::string &image_path, bool flip_texture,
                 bool top_left_coords = false);
    void load_atlas(const std::string &json_path, const std::string &image_path);
    std::vector<glm::vec2> get_texture_coordinates_of_sub_texture(const std::string &sprite_name) const;
    unsigned int get_texture_name() const;
    void bind_texture() const;
    bool using_top_left_coords;

  private:
    std::vector<glm::vec2> compute_uv_coordinates(const SpriteInfo &sprite) const;
    void load_texture(const std::string &image_path);

    std::map<std::string, SpriteInfo> sprite_map; // maps sprite names to their info
    unsigned int texture;                         // opengl texture id
    int atlas_width;                              // width of the atlas texture
    int atlas_height;                             // height of the atlas texture
};

#endif // TEXTURE_ATLAS_HPP
