#!/bin/bash

DATA_DIR='new_data'
OUTPUT_DIR='new_data_results'
THRESHOLD=50
INPUT_NUM=9
OUTPUT_NUM=2

echo "Stride detection and feature selection on turning data files..."
./main $DATA_DIR/slow_run.csv $OUTPUT_DIR/slow_run_pt.csv $OUTPUT_DIR/slow_run_st.csv $OUTPUT_DIR/slow_run_maxmin.csv $OUTPUT_DIR/slow_run_feature.csv $THRESHOLD

./main $DATA_DIR/fast_run.csv $OUTPUT_DIR/fast_run_pt.csv $OUTPUT_DIR/fast_run_st.csv $OUTPUT_DIR/fast_run_maxmin.csv $OUTPUT_DIR/fast_run_feature.csv $THRESHOLD

printf "\n"
echo "Stride detection and feature selection on turning test files..."
./main $DATA_DIR/slow_run_test.csv $OUTPUT_DIR/slow_run_test_pt.csv $OUTPUT_DIR/slow_run_test_st.csv $OUTPUT_DIR/slow_run_test_maxmin.csv $OUTPUT_DIR/slow_run_test_feature.csv $THRESHOLD

./main $DATA_DIR/fast_run_test.csv $OUTPUT_DIR/fast_run_test_pt.csv $OUTPUT_DIR/fast_run_test_st.csv $OUTPUT_DIR/fast_run_test_maxmin.csv $OUTPUT_DIR/fast_run_test_feature.csv $THRESHOLD

printf "\nGenerate train files...\n"
./train_file_generator $OUTPUT_DIR/slow_run_feature.csv $OUTPUT_DIR/fast_run_feature.csv $OUTPUT_DIR/train.txt $OUTPUT_DIR/scale.txt $INPUT_NUM $OUTPUT_NUM

printf "\nGenerate test files...\n"
./test_file_generator $OUTPUT_DIR/slow_run_test_feature.csv $OUTPUT_DIR/fast_run_test_feature.csv $OUTPUT_DIR/scale.txt $OUTPUT_DIR/test.txt $INPUT_NUM $OUTPUT_NUM

printf "\nTrain neural network...\n"
./train_neural_net $OUTPUT_DIR/train.txt $INPUT_NUM $OUTPUT_NUM

printf "\nTest neural network...\n"
./test_neural_network $OUTPUT_DIR/train.txt
