# All for one one for All

This repository belongs to my master thesis in microdata analysis with an emphasis on business intelligence. It is in the area of artificial intelligence, and its methodology is based on neural networks and genetic algorithm. I have done it in the autumn semester of 2020, and the final report is available on the diva portal at the following link:

https://www.diva-portal.org/smash/get/diva2:1525202/FULLTEXT01.pdf

Also, you can directly download it from the repository.

The folders follow the report's structure, and therefore if you read it (especially the second and third chapters), you will understand how they are organized. However, to make it easier for you, I have explained them briefly in the following table:


| Foder | Description |
| --- | --- |
| Experiments | The codes that we used for making the thesis experiments are located in this folder. The project is done by the usage of the Modular Agent-Based Evolution platform (MABE) framework, which is written in C++. The main part which defines the experiment is in OFAAFOWorld folder that is located in the world directory. To understand the development, you should check the MABE's documentation on this link: https://github.com/Hintzelab/MABE/wiki |
| Data | It contains the data that I gained from the experiments using the MABE framework. It has four subfolders. To understand the CSV files' structure (in beep, mov and lod folders), you should read chapter 2.2 (Data Description and Format of Output Files) of the report. All the CSV files in those three subfolders are modified according to the research methodology. In the folder called "original," you can find the untouched files. |
| Methods |  This folder includes the Python codes that I made for converting our data to information. There are three files, and each of them is for one of the CSV file groups (mov, beep and lod). In the test subfolder, similar codes are created to work on a small percentage of data for testing propose.|
| Results | As its name suggests, it has the results created from Python codes using our data. Three subfolders are corresponding to the three categories (beep, mod, and lod). The graph folder contains evolution graphs that I made using the lod files. |

