import xml.etree.ElementTree as ET






xmlTree = ET.parse('pacmantree.xml')
xmlRoot = xmlTree.getroot()

root = (xmlRoot[0])[0]

xmlTree = ET.parse('pacmantree.xml')
xmlRoot = xmlTree.getroot()

root = (xmlRoot[0])[0]

getBTNode(root)