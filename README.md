# texture_atlas
A system which makes working with texture atlases easier, this is designed for when you want to manually load in specific textures by hand, rather than working with meshes that already has its textures defined during the modelling process.

To use this system for every texture atlas you must have an accompanying json file of this form: 
```json
{
  "sub_textures": {
    "marked_concrete": { "x": 0, "y": 150, "width": 150, "height": 150 },
    "wood": { "x": 0, "y": 0, "width": 150, "height": 150 }
  }
}
```

the above json file specifies the top left corner of each sub-texture with the specified width and height.

# warning
Also be sure to include this in exactly one other source file or else it will not work

```cpp
#define STB_IMAGE_IMPLEMENTATION 
```
