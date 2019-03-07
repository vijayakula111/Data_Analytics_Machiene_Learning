import pandas as pd
import networkx as nx
from modulegraph.modulegraph import header
from sklearn.preprocessing import MinMaxScaler
import cPickle

df = pd.read_csv('amazon_network copy.csv',header =None)
cols = [0,1,3,2]
df1 = df[cols]
df1.columns = ['SourceId', 'DestinationID','timestamp','weight']
df1['fairness'] =1.0
df1['weight'] = df1['weight'].apply(lambda x:(((x-1)/2.0)-1))
df1['SourceId'] = 'u' + df1['SourceId'].astype(str)
df1['DestinationID'] = 'p' + df1['DestinationID'].astype(str)
print(df1)



B = nx.DiGraph()
B.add_nodes_from(df1['SourceId'], bipartite=0)
B.add_nodes_from(df1['DestinationID'], bipartite=1)


for i, row in df1.iterrows():
    B.add_edge(row[0], row[1], attr_dict=row[2:].to_dict())
edges = B.edges(data=True)
nodes = B.nodes()
#print(edges[0])
#print(B.nodes())

print "network has %d nodes and %d edges" % (len(nodes), len(edges))

nx.write_gpickle(B, "test2.pkl")
