let list = document.getElementById("todoList");

const add_ele = () => {
    let ele = document.getElementById("textBox");
    if (!ele.value)
        return;
    let li = document.createElement("li");
    li.setAttribute("class", "todoItem");
    
    let span = document.createElement("span");
    span.setAttribute("class", "todoText");
    span.appendChild(document.createTextNode(ele.value));
    li.appendChild(span);

    let btn = document.createElement("button");
    btn.appendChild(document.createTextNode("Delete"));
    btn.addEventListener("click", function () {
        li.remove();
    })
    li.appendChild(btn);

    list.appendChild(li);
    ele.value = "";
  };

const add = document.getElementById("addButton");

add.addEventListener("click", add_ele);