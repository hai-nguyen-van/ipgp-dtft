# Requires the scatterplot3d library

# declares function to check if a package exists in installed packages set
is.installed <- function(mypkg) is.element(mypkg, installed.packages()[,1])

# if package is not installed yet, fetches package files and install them
if (! (is.installed ("scatterplot3d"))){
   print ("WARNING: scatterplot3d is not installed")
   install.packages ("scatterplot3d")
}

# Arguments specified in commandArgs()
args <- (commandArgs(TRUE))
for (i in length(args) : 1){
  eval (parse (text=args[[i]]))
}

library (scatterplot3d)
spectres <- read.csv (input, header=TRUE)
# function pdf() or png() ?
png (output)
# pch=46 to have points instead of bullets
scatterplot3d (spectres, col.grid="grey", main="Signal spectrum with respect to time", grid=TRUE, angle=25, type="p")
dev.off()
# readline ("Press <Enter> to quit")