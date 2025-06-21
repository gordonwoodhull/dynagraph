// Incrface Grammar

commands = command*
command = open_graph / close_graph / modify_graph / pulse_graph / lock_graph / unlock_graph
	/ insert_node / modify_node / delete_node
    / insert_edge / modify_edge / delete_edge
    / message

DIGIT = [0-9]
LETTER = [A-Za-z_] // incrface has _\200-\377 but i can't figure out what base
NAME = a:LETTER b:(LETTER / DIGIT)* { return a + b.join(''); }
NUMBER = neg:[-]?num:((DIGIT+ ("." DIGIT*)?) / ("." DIGIT+)) { return (neg ? -1 : 1) * Number(num.flat(3).join('')); }
ESCAPED = "\"" { return '"'; }
  / "\\" { return '\\'; }
  / "n" { return '\n'; }
  / "t" { return '\t'; }
  / "r" { return '\r'; }
STRCHAR = "\\" esc:ESCAPED { return esc; }
  / char:[^"] { return char; }
QSTRING = '"' str:(STRCHAR*) '"' { return str.flat().join(''); }
ID = NAME / NUMBER / QSTRING

_ "whitespace"
  = [ \t\n\r]*
  
cr = [\n\r]*

open_graph = "open" _ "graph" _ graph:ID _ attrs:attrlist cr { return {action: 'open', kind: 'graph', graph, attrs}; }
modify_graph = "modify" _ "graph" _ graph:ID _ attrs:attrlist cr { return {action: 'modify', kind: 'graph', graph, attrs}; }
close_graph = "close" _ "graph" _ graph:ID cr { return {action: 'close', kind: 'graph', graph}; }
pulse_graph = "pulse" _ "graph" _ graph:ID _ attrs:attrlist cr { return {action: 'pulse', kind: 'graph', graph, attrs}; }
lock_graph = "lock" _ "graph" _ graph:ID cr { return {action: 'lock', kind: 'graph', graph}; }
unlock_graph = "unlock" _ "graph" _ graph:ID cr { return {action: 'unlock', kind: 'graph', graph}; }

insert_node = "insert" _ "node" _ graph:ID _ node:ID _ attrs:attrlist cr { return {action: 'insert', kind: 'node', graph, node, attrs}; }
modify_node = "modify" _ "node" _ graph:ID _ node:ID _ attrs:attrlist cr { return {action: 'modify', kind: 'node', graph, node, attrs}; }
delete_node = "delete" _ "node" _ graph:ID _ node:ID cr { return {action: 'delete', kind: 'node', graph, node}; }

insert_edge = "insert" _ "edge" _ graph:ID _ edge:ID _ source:ID _ target:ID _ attrs:attrlist cr { return {action: 'insert', kind: 'edge', graph, edge, source, target, attrs}; }
modify_edge = "modify" _ "edge" _ graph:ID _ edge:ID _ attrs:attrlist cr { return {action: 'modify', kind: 'edge', graph, edge, attrs}; }
delete_edge = "delete" _ "edge" _ graph:ID _ edge:ID cr { return {action: 'delete', kind: 'edge', graph, edge}; }

message = "message" _ message:QSTRING { return {action: 'message', message}; }

attrlist = "[" attrs:attrdefs "]" { return Object.fromEntries(attrs); }
	/ "" { return {}; }

attrdefs = attritem|.., _ "," _|

attritem = k:ID _ "=" _ v: ID { return [k,v]; }
