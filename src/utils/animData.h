#pragma once

#include<glm/glm.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

struct BoneInfo
{
	/*id is index in finalBoneMatrices*/
	int id;

	/*offset matrix transforms vertex from model space to bone space*/
	glm::mat4 offset;

};

class AnimatorParser {
public:
    // Function to parse a line of floats
    static std::vector<float> parseFrame(const std::string& line) {
        std::vector<float> result;
        std::istringstream iss(line);
        float value;
        while (iss >> value) {
            result.push_back(value);
        }
        return result;
    }

    // Function to read the input and parse each line
    static std::vector<std::vector<float>> parseAnimationData(const std::string& filename) {
        std::vector<std::vector<float>> result;
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return result;
        }
        std::string line;
        while (std::getline(file, line)) {
            std::vector<float> floatArray = parseFrame(line);
            result.push_back(floatArray);
        }
        file.close();
        // Transpose the result
        result = transpose(result);
        return result;
    }

private:
    // Function to transpose a matrix
    static std::vector<std::vector<float>> transpose(const std::vector<std::vector<float>>& matrix) {
        if (matrix.empty()) {
            return {};
        }
        size_t numRows = matrix.size();
        size_t numCols = matrix[0].size();
        std::vector<std::vector<float>> result(numCols, std::vector<float>(numRows));
        for (size_t i = 0; i < numCols; ++i) {
            for (size_t j = 0; j < numRows; ++j) {
                result[i][j] = matrix[j][i];
            }
        }
        return result;
    }
};

#pragma once
