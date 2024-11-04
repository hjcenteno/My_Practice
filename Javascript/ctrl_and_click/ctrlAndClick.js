function hello(e) {
    if (e.ctrlKey) {
        console.log("success!");
        e.onmousedown = startDrag(e);
    }
}

function startDrag(e) {
    let rect = document.querySelector("rect");

    let offsetX = e.clientX - rect.getBoundingClientRect().left;
    let offsetY = e.clientY - rect.getBoundingClientRect().top;
    if(rect){
        console.log("yippee!");
    }

    document.onmousemove = move(e, rect, offsetX, offsetY);
    
    function move(e, elmnt, offsetX, offsetY) {
        x = offsetX - e.clientX;
        y = offsetY - e.clientY;
        rect.setAttribute("x", x);
        rect.setAttribute("y", y);
    }
    
}