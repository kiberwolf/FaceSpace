#include "classifier.h"

const double Classifier::MAX_DIST = 1e9;
const double Classifier::THRESHOLD = 0.11;

Classifier::Point::Point() {}
Classifier::Point::Point(vector<double> &coords, __int64 uid, __int64 number)
    : coords(coords), uid(uid), number(number) {}

double Classifier::distance(vector<double> &v1, vector<double> &v2) {
    assert(v1.size() == v2.size());
    double sum = 0;
    for (int i = 0; i < v1.size(); ++i) {
        sum += sqr(v1[i] - v2[i]);
    }
    return sqrt(sum);
}

__int64 Classifier::getUID(vector<double> &point) {
    __int64 minDistUID = -1;
    __int64 minDistNumber = -1;
    double minDist = MAX_DIST;
    for (int i = 0; i < points.size(); ++i) {
        double dist = distance(point, points[i].coords);
        if (dist < minDist) {
            minDist = dist;
            minDistUID = points[i].uid;
            minDistNumber = points[i].number;
        }
    }

    __int64 uid;
    if (minDist > THRESHOLD) {
        uid = ++prevUniqueUID;
    } else {
        uid = minDistUID;
    }
    //lastMinDist = minDist;
    //lastNearestNumber = minDistNumber;
    cout << "    minDist: " << minDist << "   UID: " << uid 
        << "   minDistUID: " << uid << "    number: " << ++number 
        << "   minDistNumber: " << minDistNumber << endl;
    points.push_back( Point(point, uid, number) );
    return uid;
}

bool Classifier::isSame(vector<double> &v1, vector<double> &v2) {
    cout << distance(v1, v2) << endl;
    if (distance(v1, v2) <= THRESHOLD) {
        return true;
    } else {
        return false;
    }
}

/*double Classifier::getLastMinDist() {
    if (lastMinDist != MAX_DIST)  {
        return lastMinDist;
    } else {
        return 0;
    }
}

__int64 Classifier::getNearestNumber() {
    if (lastNearestNumber == -1) {
        return 1;
    } else {
        return lastNearestNumber;
    }
}*/