const dom = {
    new: document.getElementById("new"),
    add_bottom: document.getElementById("add_bottom"),
    tasks: document.getElementById("tasks"),
    count: document.getElementById("count")
}
//array tasks
const tasks = [];
//add task
dom.add_bottom.onclick = () => {
    //alert("Add new nevTaskText!");
    const nevTaskText = dom.new.value
    if (nevTaskText && isNotHaveTask(nevTaskText, tasks)) {
        addTask(nevTaskText, tasks);
        dom.new.value = "";
        tasksRender(tasks);
    }

}

//Добавление задачи
function addTask(text, list) {
    const timestamp = Date.now()
    const task = {
        id: timestamp,
        text,
        isComplete: false
    }
    list.push(task)

}


//task existence check
function isNotHaveTask(text, list) {
    let isNOTHave = true;


    list.forEach((task) => {
        if (task.text == text) {
            alert("Task existence yet")
            isNOTHave = false;
        }
    })
    return isNOTHave;
}

//function  output task
function tasksRender(list) {
    let htmlList = '';

    list.forEach((task) => {
        const cls = task.isComplete
            ? 'todo_task todo_task_complete'
            : 'todo_task'
        const checked = task.isComplete ? 'checked' : ''
        const taskHtml =
            `<div id="${task.id}" class"${cls}">
            <label class='todo_checkbox'>
                <input type="checkbox" ${checked}>   
                <div class='todo_checkbox-div'></div>
            </label>
            <div class='todo_task_text'>${task.text}</div>
            <div class='todo_task_del'>-</div>
        </div>`

        htmlList = htmlList + taskHtml;
    })
    dom.tasks.innerHTML = htmlList;
    renderTasksCount(list);
}

//status task
dom.tasks.onclick = (event) => {
    const target = event.target;
    const isCheckboxEl = target.classList.contains('todo_checkbox-div');
    const isDeleteEl = target.classList.contains('todo_task_del');
    if (isCheckboxEl) {
        const task = target.parentElement.parentElement
        const taskId = task.getAttribute('id');
        changeTaskStatus(taskId, tasks);
        tasksRender(tasks);
    }
    if (isDeleteEl) {
        const task = target.parentElement;
        const taskId = task.getAttribute('id');
        deleteTask(taskId, tasks);
        tasksRender(tasks);
    }
}

//Function change status task
function changeTaskStatus(id, list) {
    list.forEach((task) => {
        if (task.id == id) {
            task.isComplete = !task.isComplete;
        }
    })
}

//Function delete tasks
function deleteTask(id, list) {
    list.forEach((task, id) => {
        if (task.id == id) {
            list.splice(idx, 1);


        }
    })
}

// count tasks
function renderTasksCount(list) {
    dom.count.innerHTML = list.length;

}