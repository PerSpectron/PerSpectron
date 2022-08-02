/*
Fast Artificial Neural Network Library (fann)
Copyright (C) 2003-2012 Steffen Nissen (sn@leenissen.dk)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <stdio.h>
#include <string.h>

#include "fann.h"

int FANN_API test_callback(struct fann *ann, struct fann_train_data *train,
	unsigned int max_epochs, unsigned int epochs_between_reports,
	float desired_error, unsigned int epochs)
{
	printf("Epochs     %8d. MSE: %.5f. Desired-MSE: %.5f\n", epochs, fann_get_MSE(ann), desired_error);
	return 0;
}

int main(int argc, char *argv[])
{
	float threshold = (float) atof(argv[1]);
	char *train_data_file = argv[2];
	char *test_data_file = argv[3];

	printf("Threshold: %f, Train Data: %s\n", threshold, train_data_file);

	fann_type *calc_out;
unsigned int num_input =  98;
const unsigned int num_output = 2;
	const unsigned int num_layers = 2;
const unsigned int num_neurons_hidden = 1;
	const float desired_error = (const float) 0;
	const unsigned int max_epochs = 1000;
	const unsigned int epochs_between_reports = 100;
	struct fann *ann;
	struct fann_train_data *train_data;
	struct fann_train_data *test_data;

	unsigned int i = 0;
	unsigned int decimal_point;

	printf("Creating network.\n");
	ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_output);

	train_data = fann_read_train_from_file(train_data_file);

	fann_set_activation_steepness_hidden(ann, 1);
	fann_set_activation_steepness_output(ann, 1);

	fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
	fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

	fann_set_train_stop_function(ann, FANN_STOPFUNC_BIT);
	fann_set_bit_fail_limit(ann, 0.01f);
//	fann_set_bit_fail_limit(ann, 0.4f);

	fann_set_training_algorithm(ann, FANN_TRAIN_RPROP);

	fann_init_weights(ann, train_data);

	printf("Training network.\n");
	fann_train_on_data(ann, train_data, max_epochs, epochs_between_reports, desired_error);
//	fann_train_on_data(ann, train_data, max_epochs, epochs_between_reports);


	test_data = fann_read_train_from_file(test_data_file);	

	printf("Testing network. %f\n", fann_test_data(ann, test_data));
	int correct=0;
	int total=0;
	for(i = 0; i < fann_length_train_data(test_data); i++)
	{
		total++;
		calc_out = fann_run(ann, test_data->input[i]);
		float output;
		if (calc_out[0] < threshold)
			output = -1.0;
		else
			output = 1.0;
		printf("prediction test %d-> (net_out: %f), (out: %f), should be (%f)  difference=(%f)\n",
		i, calc_out[0], output,
		 test_data->output[i][0],
		 fann_abs(output - test_data->output[i][0]));
		//	   data->input[i][0], data->input[i][1], calc_out[0], data->output[i][0],
			//   fann_abs(calc_out[0] - data->output[i][0]));
		if(output - test_data->output[i][0]==0)
			correct++; 
	}
		
	double accuracy=(double)correct/total;
	printf ("accuracy=%lf\n",accuracy);
	printf("Saving network.\n");

	fann_save(ann, "spectre.net");


	//decimal_point = fann_save_to_fixed(ann, "spectre.net");
	//fann_save_train_to_fixed(train_data, "trained.data", decimal_point);

	printf("Cleaning up.\n");
	fann_destroy_train(train_data);
	fann_destroy(ann);

	return 0;
}
