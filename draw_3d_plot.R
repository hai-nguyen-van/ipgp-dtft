# Requires the scatterplot3d library
# > install.packages ("scatterplot3d")

# Arguments specified in (commandArgs())[n], n ≥ 2
args <- (commandArgs(TRUE))
for (i in length(args) : 1){
  eval (parse (text=args[[i]]))
}

library (scatterplot3d)
spectres <- read.csv (fichier, header=TRUE)
scatterplot3d (spectres, col.grid="grey", main="Signal spectrum with respect to time", grid=TRUE, angle=25, pch=46, type="p")

# readline ("Press <Enter> to quit")