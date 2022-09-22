# Citation
A. Pashrashid, A. Hajiabadi, T. E. Carlson, "Fast, Robust and Accurate Detection of Cache-based Spectre
Attack Phases", 41st IEEE/ACM International Conference on Computer-Aided Design (ICCAD '22) 

# How to Run Perspectron


Step 1: Buiilding and running gem5 to dump performance counters every 10k instructions:
--------------------------------------------------------------------------------------------

*Command:*

	$ cd $GEM5_PATH
	$ git clone
	$ cd gem5-PerSpectron
	$ scons build/X86/gem5.opt
	$ $GEM5_PATH/gem5-PerSpectron/build/X86/gem5.opt -d [stats_path]                         \
	configs/example/se.py --bp-type=PerceptronLocalBP --cpu-type DerivO3CPU \
	--l1d_size 32kB --l1d_assoc 8 --l1i_size 32kB --l1d_assoc 8             \
	--l2_size 2MB --l2_assoc 16 --l2cache --caches --cmd [binary]           \

-   After running this command for \[binary\], the stats will be in
    \[stats\_path\] folder.

-   \$GEM5\_PATH is the directory that gem5 is built.

**Example for benign:**

	$ $GEM5_PATH/build/X86/gem5.opt -d m5out/qsort configs/example/se.py \
	--bp-type=PerceptronLocalBP --cpu-type DerivO3CPU --l1d_size 32kB.   \
	--l1d_assoc 8 --l1i_size 32kB --l1d_assoc 8 --l2_size 2MB            \
	--l2_assoc 16 --l2cache --caches --cmd examples/qsort -o             \
	examples/input_large.dat                                             \

**Example for malicious:**

	$ $GEM5_PATH/build/X86/gem5.opt -d m5out/spectre                        \
	configs/example/se.py --bp-type=PerceptronLocalBP --cpu-type DerivO3CPU \
	--l1d_size 32kB --l1d_assoc 8 --l1i_size 32kB --l1d_assoc 8             \
	--l2_size 2MB --l2_assoc 16 --l2cache --caches --cmd                    \
	examples/spectre.out

Step 2: Running sample preparation to generate normalized performance counters in a readable format by FANN (For both malicious and benign programs)
----------------------------------------------------------------------------------------------------------------------------------------------------

*Commands:*

For malicious programs:
	
	$ python sample_preparation_malicious.py [path-to-params] \
	[path-to-stats] > [samples_file]

**Example**:

	$ python sample_preparation_malicious.py examples/params-99
	m5out/spectre/stats.txt > examples/spectre-samples

For benign programs:

	$ python sample_preparation_benign.py [path-to-params] \
	[path-to-stats] > [samples_file]

**Example**:

	$ python sample_preparation_benign.py examples/params-99
	m5out/qsort/stats.txt > examples/qsort-samples

-   For running these commands you need to provide a list of
    Perspectron's performance counters (An example list is
    provided: params-99. Refer to the Perspectron papers for
    more details).

-   The samples will be in the \[samples\_file\] path.

-   The difference between these two commands is the label of
    the samples.

Step 3: Running Perspectron
---------------------------

### Step 3-1: If you have a trained model and only want to test the model

*Command:*

	$ ./test_spectre [threshold] [samples_file]

**Example**:

	$ ./test_spectre 0.25 examples/spectre-samples

-   Make sure that your trained model is in a file named "spectre.net".

-   Also you need to provide the threshold number to differentiate
    between malicious and benign programs. (Refer to the Perspectron
    papers for more details. The default value from the paper is 0.25).

-   As the output you will see the model output for each test sample and
    also the accuracy of the model will be reported at the end (The
    accuracy is the number correctly predicted labels for test sample
    divided by the total number of test samples)

-   Use "make" command to build the C codes.

### Step 3-2: If you want to first train and then test the model

#### Step 3-2-1: Partitioning the generated samples into train and test dataset

*Command:*

	$ python samples_partitioning.py [sample_file]

**Example**:

	$ samples_partitioning.py spectre-samples

-   The output will be two files with .train and .test data (66% of the
    data used for training and the remaining for test)

#### 

#### Step 3-2-2: Performing the train and test

*Command:*

	$ ./train_test_spectre [threshold] [train data] [test data]

**Example**:

	$ ./train_test_spectre 0.25 examples/spectre-samples
	examples/spectre-samples

-   Provide the train and test datasets generated in the previous step
    as input.

-   The output will be the same as step 3.a and the trained model will
    be save as "spectre.net".




