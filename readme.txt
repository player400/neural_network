The purpose of this project is to create a working neural network from scratch in C++ and train it with genetic algorithm.
______________________________________________________________________________________________________________________________________________________________________________________

First goal was to train a small (3 layers) model to recognise if a color is red or not, based on the color's RGB code.
It was achieved succesfully, with the best models reaching about 96% accuracy. A leftover from this task is the colors.h file.
It contains a class that was used to load a dataset of colors. Code that was used to train those models (Trainer class) has been changed and is no longer compatible with this data format.
In the future I may bring it back as a separate branch.

Current task is to train a model for recognising handwritten digits from 14x14 monochromatic images.
The best model (attached as example.network) has managed to reach about 45% accuracy, while being tested on the same dataset it was trained on.
It has 3 layers (including 1 hidden layer) of sizes: 196, 25 and 10 nodes.

Getting the network to reach high accuracy in recognising all ten digits was so far unsuccesfull. 
However a model can be easily trained to recognise one specific digit from others (for example model would almost perfectly flag all 1s as 1s and flag all other digits as 0s).
In fact, while attempting to train a model it often naturally defaulted to a similar setup. 



Guide for testing the software yourself:
______________________________________________________________________________________________________________________________________________________________________________________

After starting the program it will automatically import the dataset from digit_dataset.csv file.
It will then create a new empty network and upload it into a trainer (object of class Trainer, that uses genetic algorithm to train the model).
Trainer is operated from command lines, with following commands:


test <n> - tests the current model against every n-th example in the dataset. This command will print detailed breakdown on how did the network perform in recognising each of 10 digits.
Currently there is no way to use separate training and testing datasets, unless you swap files manually. It is to be changed.

train <n> - trains current model (n generations)

try <n> <i1><i2>...<in> - feeds example given by a user to the current model, n needs to be equal to size of the first layer of the model 
(rather useless for digits, since the user would need to manually insert brightness 196 pixels, it was usefull for colors)

display - prints values of all biases and connections in the neural network. Usefull for small models, unreadable for larger ones.

try_random - tests the network against a random example from the dataset. 

kill_parents - deletes all networks from the current parent generation, except for the best one. next time the ,,train" command is called a new parent generation will be generated based on the surviving one.

save <name> - saves current network under name.network 

load <name> - loads a model from name.network and loads it into the trainer

NOTE: Only the best model from the current parent generation is saved - rest of the parent population is deleted after closing the program.


Currently there is no config file. Properties of the program may be modified by editing macros (#define) located in the main.cpp file. This includes things line population size.
One of the most important things there is number of threads this program will be running on. It is currently set to 4 threads. You should adjust it to your machine.



Plans for the future
______________________________________________________________________________________________________________________________________________________________________________________

Since genetic approach for training digit recognition model proved unsuccesfull, in the future I plan on switching to back propagation. I am currently in the process of reading research papers on the subject and working out the math.
Unfortunately I am restricted by hardware (2-core i3 laptop with integrated graphics).



Final notes
______________________________________________________________________________________________________________________________________________________________________________________

While developing this project I was not using Git/GitHub. That's why I am pushing it all at once and there is no version history for it. 

For the same reason large part of the code used to train networks for color recognition no longer exists. Trainer class had to be modified to accomodate other type of inputs and outputs of the network.
If I were to do it all again, old version would be saved in a separate branch, but it isn't.
At some point I may write it again and add as a separate branch.
