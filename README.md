# Image Fusion Application

This application performs image fusion using various methods. 

NOTE: this version is 1.2.0 with opencv integrated

- IHS method (OpenCV working)
- MEAN method (OpenCV working)
- BROVEY method (there is a bug in QpenCV brovey method I will debug later.)
- ESRI method (not yet implemented update later)

NOTE: version 1.0.0 (working all modules) available for free on techOpswalk.
[Releases](https://github.com/techopswalk/Image-Fusion-Application/releases/tag/Image-Fusion) page.

## Dependencies
- QT: You need to have QT installed to build and run this application.
- OpenCV: You need to have OpenCV installed to build and run this application.

## Functionalities

- **Image Fusion**: The application allows you to fuse different bands of images using different fusion methods, such as IHS and Brovey.
- **Save Fused Image**: You can save the fused image to your filesystem.

## Build Instructions

Follow these steps to build and run the application using CMake:

1. Clone the repository:

```bash
git clone https://github.com/yourusername/image-fusion-app.git
cd image-fusion-app
```

2. Create a build directory and navigate to it:
```
mkdir build
cd build
```
3. Configure the build with CMake:
```
cmake ..
```
4. Build the application:
```
cmake --build .
```

![modified_000007](https://github.com/pahuldeep/ImageFusion/assets/52893813/691eb615-a13c-4716-a525-6e8b75d4a81d)

