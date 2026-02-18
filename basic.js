//Counter variable
let counter = 0;

//1pt: Simple functions
function tickUp(){
  counter++;
  document.getElementById("counter").textContent = counter;
}

function tickDown(){
  counter--;
  document.getElementById("counter").textContent = counter;
}

//1pt: Simple for loop
function runForLoop(){
  let result = "";
  for (let i = 0; i <= counter; i++){
    result += i + " ";
  }
  document.getElementById("forLoopResult").textContent = result.trim();
}

//1pt: Repetition with condition
function showOddNumbers(){
  let result = "";
  for (let i = 1; i <= counter; i++){
    if (i % 2 !== 0){
      result += i + " ";
    }
  }
  document.getElementById("oddNumberResult").textContent = result.trim();
}

//1pt: Arrays
function addMultiplesToArray() {
  let arr = [];
  for (let i = 5; i <= counter; i += 5){
    arr.push(i);
  }
  arr.reverse();
  console.log(arr);
}

//2pts: Objects and form fields
function printCarObject(){
  let carObj ={
    cType: document.getElementById("carType").value,
    cMPG: document.getElementById("carMPG").value,
    cColor: document.getElementById("carColor").value
  };
  console.log(carObj);
}

//2pts: Objects and form fields pt. 2
function loadCar(num){
  let car;
  if (num === 1) car = carObject1;
  else if (num === 2) car = carObject2;
  else if (num === 3) car = carObject3;

  document.getElementById("carType").value = car.cType;
  document.getElementById("carMPG").value = car.cMPG;
  document.getElementById("carColor").value = car.cColor;
}

//2pt: Changing styles
function changeColor(num){
  let p = document.getElementById("styleParagraph");
  if (num === 1) p.style.color = "red";
  else if (num === 2) p.style.color = "green";
  else if (num === 3) p.style.color = "blue";
}