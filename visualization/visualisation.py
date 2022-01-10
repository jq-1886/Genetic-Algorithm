from graphviz import Digraph

with open('circuit_vector.txt') as f:
    circuit_array = [[int(x) for x in line.split()] for line in f]

num_unit = int((len(circuit_array[0]) - 1) / 2)

dot = Digraph()
dot.attr(rankdir='LR')
dot.attr('node', shape='rectangle')

source = Digraph('source')
source.graph_attr.update(rank='source')
sink = Digraph('sink')
sink.graph_attr.update(rank='sink')

source.node("-1", "Feed")
sink.node(str(num_unit), "concentrate", rank='sink')
sink.node(str(num_unit + 1), "tailing", rank='sink')

dot.edge("-1", str(circuit_array[0][0]), headport='w', tailport='e')

for i in range(int((len(circuit_array[0]) - 1) / 2)):
    dot.edge(str(i), str(circuit_array[0][2 * i + 1]),
             color='blue', headport='w', tailport='n')
    dot.edge(str(i), str(circuit_array[0][2 * i + 2]),
             color='red', headport='w', tailport='s')

dot.subgraph(source)
dot.subgraph(sink)

dot.render(filename='circuit', cleanup=True, format='png')
