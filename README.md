# kDTree_Final_Project
Project Overview
MNIST-kNN-Classifier is a C++-based machine learning system designed to classify handwritten digits (0–9) from the MNIST dataset. It implements the k-Nearest Neighbors (kNN) algorithm, enhanced with a kD-Tree data structure to optimize nearest neighbor searches, achieving a query complexity of O(log n). The project includes custom classes for data handling (Dataset), kD-Tree operations (kDTree), and kNN classification (kNN), showcasing skills in object-oriented programming, advanced data structures, and machine learning.

Features
kNN Classification: Classifies MNIST digits using the k-Nearest Neighbors algorithm with majority voting.
kD-Tree Optimization: Implements a kD-Tree for efficient nearest neighbor searches, reducing time complexity from O(n) to O(log n).
Custom Data Handling: Features a Dataset class to load, preprocess, and split the MNIST dataset into training and test sets.
Accuracy Evaluation: Includes a score function to compute classification accuracy on the test set.
UNIX-Compatible: Built and tested using g++ with C++11 standards.

Dataset
The project uses a subset of the MNIST dataset, which contains 28x28 grayscale images of handwritten digits (0–9). Each image is represented by 784 pixel features, with a corresponding label. The dataset is provided in data/mnist.csv as a sample subset for this project.

Format: CSV file with 785 columns (1 label + 784 pixel values).
Size: [Specify the number of samples if known, e.g., 10,000 samples].

Technologies Used
Language: C++ (C++11 standard)
Version Control: Git, GitHub
Development Environment: [Specify IDE if known, e.g., Visual Studio Code, CLion]


