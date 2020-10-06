filesFolder = "/home/yellow/Desktop/Study/BI/Thesis/trials/first/"
fileName = "LOD_0_0_0.csv"
currentFile = read.csv(paste(filesFolder, fileName, sep = ""))  
names(currentFile)
print(mean(currentFile$score))







