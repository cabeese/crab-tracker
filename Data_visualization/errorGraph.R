 
 #Graph where color is informed by theta
 library(plotly)
 
 data <- read.csv("../src/pi-src/data.csv")
 p <- plot_ly(data, x = ~x, y = ~y, z = ~z,            
	marker = list(color = ~difTheta, size = 1, colorscale = c('#FFE1A1', '#683531'), showscale = TRUE)) %>%
	add_markers() %>%
		layout(scene = list(xaxis = list(title = 'x'),
                     yaxis = list(title = 'y'),
                     zaxis = list(title = 'z'))
)


 #Graph where color is informed by diffN
 library(plotly)
 
 data <- read.csv("../src/pi-src/data.csv")
 p <- plot_ly(data, x = ~x, y = ~y, z = ~z,            
	marker = list(color = ~difN, size = 1, colorscale = c('#FFE1A1', '#683531'), showscale = TRUE)) %>%
	add_markers() %>%
		layout(scene = list(xaxis = list(title = 'x'),
                     yaxis = list(title = 'y'),
                     zaxis = list(title = 'z'))
)
