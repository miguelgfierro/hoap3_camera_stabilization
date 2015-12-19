# HOAP-3 camera stabilization algorithm

This is the code for the stabilization algorithm of the camera in the humanoid HOAP-3. The algorithm computes for each frame the [SURF](https://en.wikipedia.org/wiki/Speeded_up_robust_features) descriptors and compare them with the previous frame using [Nearest Neighbour](https://en.wikipedia.org/wiki/K-nearest_neighbors_algorithm). The comparison allows to compute the relative translation and rotation of the camera in every frame. Finally an affine transformation is applied taking to smooth the movement between frames. 

In this [video](https://www.youtube.com/watch?v=qQKlUAgH7Is) the result of the algorithm is presented. The video is taken from the humanoid cameras and the robot is walking forward using [Cart-Table](https://staff.aist.go.jp/kensuke.harada/papers/2003-ICRA%28KKKFHYH%29.pdf) algorithm.

For more information please visit my [blog](http://miguelgfierro.com) or my [youtube channel](https://www.youtube.com/user/ciruselvirus).
