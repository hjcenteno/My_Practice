const ns = "http://www.w3.org/2000/svg"
const svg = document.getElementsByName("svg")

// Individual node and edge
let node;
let edge;

// Holds the nodes and edges
const nodes = [];
const edges = [];

const graph = {
    
}

// Create nodes and edges
for (let i = 0; i < 2; i++) {
    node = document.createElementNS(ns, "circle");
    nodes.push(node);

    edge = document.createElementNS(ns, "line");
    edges.push(edge);
}

// Set attributes for nodes and append them
nodes[0].setAttribute("cx", 100);
nodes[0].setAttribute("cy", 100);
nodes[0].setAttribute("r", 25);
nodes[0].setAttribute("fill", "red");
nodes[0].addEventListener("click", startDrag(nodes[0])); 
svg.appendChild(nodes[0]);

nodes[1].setAttribute("cx", 200); 
nodes[1].setAttribute("cy", 200);
nodes[1].setAttribute("r", 25);
nodes[1].setAttribute("fill", "blue");
nodes[1].addEventListener("click", startDrag(nodes[1])); 
svg.appendChild(nodes[1]);

function startDrag(elmnt) {
    document.onmouseup = closeDrag;
    document.onmousemove = drag;

    function drag(e){
        elmnt.setAttribute("cx", e.clientX);
        elmnt.setAttribute("cy", e.clientY);
    }

    function closeDrag(){
        document.onmouseup = null;
        document.onmousemove = null;
    }
}