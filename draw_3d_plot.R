# Requires the scatterplot3d library
# > install.packages ("scatterplot3d")

# Arguments specified in commandArgs()
args <- (commandArgs(TRUE))
for (i in length(args) : 1){
  eval (parse (text=args[[i]]))
}

library (scatterplot3d)
spectres <- read.csv (fichier, header=TRUE)
pdf (fichier)
scatterplot3d (spectres, col.grid="grey", main="Signal spectrum with respect to time", grid=TRUE, angle=25, pch=46, type="p")
dev.off()
# readline ("Press <Enter> to quit")