# DecisionTreeWineQualityPrediction

The aim of this C++ program is to predict the wine quality rating of a wine sample, based on object features.
The predictor function will be constructed as a decision tree, and a decision learning tree algorithm is applied to the continuous data provided as input.

Three main algorithms are utilised, the first creates a decision learning tree that recursively calls itself and creates new nodes upon evaluating specific conditions. 
The second algorithm analysed the information gain (through calculating entropy) of each data point, and returns the best possible gain value to algorithm 1. 
The last algorithm is used to predict the quality of each wine sample. 

3 inputs are required, the first should be a train file that the model will use to train with, the second will be a test file that the model will predict wine quality of,
and the third will be a minleaf value that determines how big the tree created will be. 
