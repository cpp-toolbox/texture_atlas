#include "texture_atlas.hpp"
#include <stb_image.h>
#include <fstream>
#include <iostream>
#include <glad/glad.h>

// Alias for JSON
using json = nlohmann::json;

TextureAtlas::TextureAtlas(const std::string &json_path, const std::string &image_path, bool flip_texture,
                           bool top_left_coords) {
    using_top_left_coords = top_left_coords;
    /*stbi_set_flip_vertically_on_load(1);*/
    load_atlas(json_path, image_path);
}

void TextureAtlas::load_atlas(const std::string &json_path, const std::string &image_path) {
    std::ifstream file(json_path);
    if (!file) {
        std::cerr << "Error: Unable to open JSON file: " << json_path << "\n";
        return;
    }

    json atlas_json;
    file >> atlas_json;

    auto sprites = atlas_json["sub_textures"];
    for (auto &[sprite_name, value] : sprites.items()) {
        SpriteInfo info;
        float x = value["x"];
        float y = value["y"];
        float width = value["width"];
        float height = value["height"];

        info.top_left = glm::vec2(x, y);
        info.bottom_right = glm::vec2(x + width, y + (using_top_left_coords ? 1 : -1) * height);

        sprite_map[sprite_name] = info;

        /*// Log the sprite creation*/
        /*std::cout << "Sprite Created: " << sprite_name << "\n"*/
        /*          << "  Top-Left: (" << info.top_left.x << ", " << info.top_left.y << ")\n"*/
        /*          << "  Bottom-Right: (" << info.bottom_right.x << ", " << info.bottom_right.y << ")\n";*/
    }

    // Load the texture image
    load_texture(image_path);
    std::cout << "Texture loaded from: " << image_path << "\n";
}

void TextureAtlas::load_texture(const std::string &image_path) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    int nrChannels;
    unsigned char *data = stbi_load(image_path.c_str(), &atlas_width, &atlas_height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlas_width, atlas_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture: " << image_path << std::endl;
    }
    stbi_image_free(data);
}
unsigned int TextureAtlas::get_texture_name() const { return texture; }

std::vector<glm::vec2> TextureAtlas::compute_uv_coordinates(const SpriteInfo &sprite) const {
    float u_min = sprite.top_left.x / atlas_width;
    float v_min = sprite.top_left.y / atlas_height;
    float u_max = sprite.bottom_right.x / atlas_width;
    float v_max = sprite.bottom_right.y / atlas_height;

    // Return UV coordinates
    return {
        {u_max, v_min}, // top-right
        {u_max, v_max}, // bottom-right
        {u_min, v_max}, // bottom-left
        {u_min, v_min}  // top-left
    };
}

std::vector<glm::vec2> TextureAtlas::get_texture_coordinates_of_sub_texture(const std::string &sprite_name) const {
    const SpriteInfo &sprite = sprite_map.at(sprite_name);
    auto temp = compute_uv_coordinates(sprite);

    // Log the sprite name and UV coordinates
    std::cout << "Sprite Name: " << sprite_name << "\n";
    for (size_t i = 0; i < temp.size(); ++i) {
        std::cout << "UV[" << i << "]: (" << temp[i].x << ", " << temp[i].y << ")\n";
    }

    return temp;

    /*RenderData data;*/
    /*data.uv_coords = compute_uv_coordinates(sprite);*/
    /*data.texture_name = texture; // Using single texture unit for now*/
    /**/
    /*return data;*/
}

void TextureAtlas::bind_texture() const { glBindTexture(GL_TEXTURE_2D, texture); }
