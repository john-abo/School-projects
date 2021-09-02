/* GIMP RGB C-Source image dump (forg.c) */

static const struct {
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  unsigned char	 pixel_data[36 * 36 * 2 + 1];
} frog = {
  36, 36, 2,
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370"
  "\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370\000\370",
};

