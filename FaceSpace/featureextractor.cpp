#include "featureextractor.h"
#include "networkutils.h"

#include <fstream>
#include <iostream>

const int FeatureExtractor::LAYER_TYPE[10] =  { LAYER_INPUT, 
                                               LAYER_CONV, LAYER_POOL, 
                                               LAYER_CONV, LAYER_POOL, 
                                               LAYER_CONV, LAYER_POOL, 
                                               LAYER_CONV, LAYER_POOL, 
                                               LAYER_NEIRON };
const int FeatureExtractor::SIZES::LAYER_EDGE[10] = { 174, 168, 84, 80, 40, 36, 18, 14, 7, 1 };
const int FeatureExtractor::SIZES::LAYER_MAPS[10] = { 1, 64, 64, 256, 256, 512, 512, 1024, 1024, 1024};
const int FeatureExtractor::SIZES::CONV_MAPS[5] = { 64, 4, 2, 2, 1 };
const int FeatureExtractor::SIZES::CONV_EDGE[5] = { 7, 5, 5, 5, 7 };
const int FeatureExtractor::SIZES::COMPETITIVE_FIELD_EDGE[5] = { 10, 8, 6, 4, 1 };
const double FeatureExtractor::SIZES::MIN_VAL = -1e9;

FeatureExtractor::FeatureExtractor() {
    for (int layer = 0; layer < SIZES::LAYERS_COUNT; ++layer) {
        int layerEdge = SIZES::LAYER_EDGE[layer];
        int layerMaps = SIZES::LAYER_MAPS[layer];
        Array2D item(layerEdge, layerEdge);
        for (int map = 0; map < layerMaps; ++map) {
            mapsLayers[layer].push_back(item);
        }
    }
}

double FeatureExtractor::activation(double sum) {
    return tanh(sum);
}

void FeatureExtractor::conv(vector<Array2D> &res, Array2D &prevMap, Array2D &sumMap,
                            Maximator &maximator,
                            int layerNum, int &mapNum, int convNum) {
    int convEdge = SIZES::CONV_EDGE[convNum];
    int convMaps = SIZES::CONV_MAPS[convNum];
    int layerEdge = SIZES::LAYER_EDGE[layerNum];
    double learningSpeed = baseLearningSpeed / trainEpoch;

    for (int map = 0; map < convMaps; ++map) {
        Array2D &weight = weights[convNum][mapNum];
        Array2D &convMap = res[mapNum];
        for (int i = 0; i < layerEdge; ++i) {
            for (int j = 0; j < layerEdge; ++j) {
                double sum = 0;
                for (int k = 0; k < convEdge; ++k) {
                    for (int t = 0; t < convEdge; ++t) {
                        sum += prevMap.at(i + k, j + t) * weight.at(k, t);
                    }
                }
                sum += weight.getBias();
                double y = activation(sum);

                sumMap.set(i, j, sum);
                convMap.set(i, j, y);
            }
        }

        if (isExtractorTrain) {
            maximator.clear();
            for (int i = 0; i < layerEdge; ++i) {
                for (int j = 0; j < layerEdge; ++j) {
                    maximator.add(i, j, sumMap.at(i, j));
                    /*double maximum = SIZES::MIN_VAL;
                    int maxK;
                    int maxT; 
                    
                    for (int k = -COMP_EDGE; k <= COMP_EDGE; ++k) {
                        if (k + i < 0 || k + i >= layerEdge) {
                            continue;
                        }
                        int localHeight = COMP_EDGE - abs(k);
                        for (int t = -localHeight; t <= localHeight; ++t) {
                            if (t + j < 0 || t + j >= layerEdge) {
                                continue;
                            }
                            if (maximum < sumMap.at(i + k, j + t)) {
                                maximum = sumMap.at(i + k, j + t);
                                maxK = k;
                                maxT = t;
                            }
                        }
                    } 
                    isLocalMax.set(i + maxK, j + maxT, 1);*/
                }
            }
            /*for (int i = 0; i < layerEdge; ++i) {
                for (int j = 0; j < layerEdge; ++j) {
                    if (isLocalMax.at(i, j) == 0) {
                        continue;
                    }
                    for (int k = 0; k < convEdge; ++k) {
                        for (int t = 0; t < convEdge; ++t) {
                            double x = prevMap.at(i + k, j + t);
                            double w = weight.at(k, t);
                            double delta = learningSpeed * (x - w);
                            weight.add(k, t, delta);
                        }
                    }
                }
            }*/
            for (int i = 0; i < maximator.size(); ++i) {
                int maxX;
                int maxY;
                maximator.getXY(i, maxX, maxY);
                for (int k = 0; k < convEdge; ++k) {
                    for (int t = 0; t < convEdge; ++t) {
                        double x = prevMap.at(maxX + k, maxY + t);
                        double w = weight.at(k, t);
                        double delta = learningSpeed * (x - w);
                        weight.add(k, t, delta);
                    }
                }
            }

            weight.normalize();
        }

        if (isExtractorTrain) {
            if (convNum != SIZES::CONV_LAST_LAYER_NUM) {
                convMap.normalize();
            }
        }
        ++mapNum;
    }
}

void FeatureExtractor::pool(Array2D &poolMap, Array2D &prevMap,
                            int layerNum) {
    int layerEdge = SIZES::LAYER_EDGE[layerNum];
    for (int i = 0; i < layerEdge; ++i) {
        for (int j = 0; j < layerEdge; ++j) {
            double maximum = max(prevMap.at(i * 2, j * 2), 
                                 prevMap.at(i * 2, j * 2 + 1));
            maximum = max(maximum, prevMap.at(i * 2 + 1, j * 2));
            maximum = max(maximum, prevMap.at(i * 2 + 1, j * 2 + 1));
            poolMap.set(i, j, maximum);
        }
    }
}

vector<double> FeatureExtractor::getVector(Mat &mat, bool visualize) {
    isVisualize = visualize;
    vector<double> &ans = getVector(mat);
    isVisualize = visualize;
    return ans;
}

vector<double> FeatureExtractor::getVector(Mat &image) {
    Mat mat;
    image.copyTo(mat);
    resize(mat, mat, Size(SIZES::LAYER_EDGE[0], SIZES::LAYER_EDGE[0]));

    Array2D inputArr(mat, isExtractorTrain);
    Maximator maximator;
    int convNum = 0;
    mapsLayers[0].clear();
    mapsLayers[0].push_back(inputArr);

    for (int layer = 1; layer < SIZES::LAYERS_COUNT; ++layer) {
        int layerMaps = SIZES::LAYER_MAPS[layer - 1];
        int layerEdge = SIZES::LAYER_EDGE[layer - 1];
        
        if (LAYER_TYPE[layer] == LAYER_CONV ||
                LAYER_TYPE[layer] == LAYER_NEIRON) {
            Array2D sumMap(layerEdge, layerEdge);
            int mapNum = 0;
            for (int map = 0; map < layerMaps; ++map) {
                conv(mapsLayers[layer], mapsLayers[layer - 1][map], sumMap,
                     maximator,
                     layer, mapNum, convNum);
            }
            ++convNum;
        } else if (LAYER_TYPE[layer] == LAYER_POOL) {
            for (int map = 0; map < layerMaps; ++map) {
                pool(mapsLayers[layer][map], mapsLayers[layer - 1][map], layer);
            }
        }
    }
    
    if (isVisualize) {
        Visializer visualizer;
        visualizer.show(mapsLayers);
    }

    vector<double> ans;
    // Does layers[lastLayerNum].size() cashing by compiler?
    int lastLayerNum = SIZES::LAYERS_COUNT - 1;
    for (unsigned int i = 0; i < mapsLayers[lastLayerNum].size(); ++i) {
        ans.push_back(mapsLayers[lastLayerNum][i].at(0, 0));
    }

    

    return ans;
}

void FeatureExtractor::train(TrainMode MODE) {
    if (MODE == TrainMode::New) {
        remove("weights.dat");
        weights = *(new Weights());
    }
    isExtractorTrain = true;

    int maxTrainSetSize = 300;
    trainEpoch = 1;

    vector<string> images = NetworkUtils::loadTrainSet();

    for (int i = 0; i < images.size(); ++i) {
        if (trainEpoch > maxTrainSetSize) {
            break;
        }
        Mat image = imread(images[i].c_str(), CV_LOAD_IMAGE_UNCHANGED);
        //if (image.rows >= SIZES::LAYER_EDGE[0]) {
        getVector(image);
        cout << trainEpoch << endl;
            
        ++trainEpoch;
        //}
    }

    isExtractorTrain = false;
}