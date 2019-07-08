import csv
import folium

m = folium.Map(location = [39.8282, -98.5795], zoom_start = 5)

with open('Cities.csv') as file:
  reader = csv.reader(file)
  for row in reader:
    folium.Marker([float(row[1]), float(row[2])],
    popup="<strong>Population: " + row[3] + "</strong>",
    tooltip = "<strong>" + row[0] + "</strong>").add_to(m)
with open('Connections.csv') as file:
  reader = csv.reader(file)
  for row in reader:
    folium.PolyLine([[float(row[0]),float(row[1])],[float(row[2]),float(row[3])]], weight=1.75, opacity=0.75, tooltip = "<strong>" + "Length: " + row[4] + "</strong>").add_to(m)
print('Saving...')
m.save('Map.html')
print("End of execution...")
