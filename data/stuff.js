var buttonWait = document.getElementById("s3");
buttonWait.onclick = async function(e) {
    //the currently selected option
    let currentMinutes = document.getElementById("time3m").value;
    
    //Maybe add error handling here
    fetch("/WAIT_TIME", {
        method: "POST",
        headers: {
            MINUTES: currentMinutes
        }
    }).then(e=>e.text().then(console.log));
}