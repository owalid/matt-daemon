const submit_btn = document.getElementById("send")
let timeout = false

const display_feedback = (type, msg) => {
  const feedback = document.getElementById("feedback-msg")
  feedback.style.display = "block"
  feedback.classList.add(type)
  feedback.textContent = msg

  if (!timeout) {
    timeout = setTimeout(() => {
      feedback.style.display = "none"
      feedback.textContent = ""
      feedback.classList.remove(type)
      timeout = false
    }, 5000)
  }
}

submit_btn.addEventListener("click", async () => {
  const feedback_error_msg = "An error occurred, please verify the api is running correctly and the daemon is running"
  try {
    const content = document.getElementById("content").value
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
})


const addCommandIntoInput = (e) => {
  const command = e.target.textContent.slice(0, -2)
  const input = document.getElementById("content")
  input.value = command
}

const commands_list = document.querySelectorAll(".command-list-item span")
commands_list.forEach(command => {
  command.addEventListener("click", addCommandIntoInput)
})