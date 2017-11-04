HOW TO RUN

Basic People detection using human interaction based on Open CVs HOGDescriptor. A more sophisticated approach could use Semantic Segmentation based network trained on MSCOCO using Inception-v3 or DenseNet. To use - 

1) Make sure you have Opencv C++ libraries installed and linked.
2) Execute ‘ppl_detector_hog.cpp’, cin Image_name, (or Hard Code).
3) OpenCV window will show up, with suggested green colored boxes as potential human figures. 
4) Pressing ‘y’ will color the candidate rectangle black, and ’n’ would ignore it.
5) The final image is saved as ‘output.jpg’.
