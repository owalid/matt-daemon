const submit_btn = document.getElementById("send")
const text_area = document.getElementById("content")
let timeout = false

const send_command = async () => {
  const feedback_error_msg = "An error occurred, please verify the api is running correctly and the daemon is running"
  try {
    const content = text_area.value
    if (content === "") {
      display_feedback("error", "Please enter content.")
      return
    }
    let res = await fetch("http://127.0.0.1:5000/api/post_input",
    {
      method: "POST",
      headers: {
        "Content-Type": "application/json"
      },
      body: JSON.stringify({"content": content})
    })
    res = await res.json()

    if (res.success) {
      display_feedback("success", "The content has been successfully sent to the daemon")
      return
    }
    display_feedback("error", feedback_error_msg)
  }
  catch (_err) {
    display_feedback("error", feedback_error_msg)
  } 
}

const display_feedback = (type, msg) => {
  const feedback = document.getElementById("feedback-msg")
  if (!timeout) {
    feedback.style.display = "block"
    feedback.classList.add(type)
    feedback.textContent = msg
    timeout = setTimeout(() => {
      feedback.style.display = "none"
      feedback.textContent = ""
      feedback.classList.remove(type)
      timeout = false
    }, 5000)
  }
}


text_area.addEventListener("keydown", (e) => {
  // cmd + enter
  if (e.keyCode === 13 && e.metaKey) {
    send_command()
  }
})

submit_btn.addEventListener("click", send_command)


const addCommandIntoInput = (e) => {
  const command = e.target.textContent.slice(0, -2)
  const input = document.getElementById("content")
  input.value = command
}

const commands_list = document.querySelectorAll(".command-list-item span")
commands_list.forEach(command => {
  command.addEventListener("click", addCommandIntoInput)
})