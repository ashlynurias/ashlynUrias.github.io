// open and close filter form for “filter articles” button
function showFilter() {
    var filterForm = document.getElementById("filterContent");
    var addForm    = document.getElementById("newContent");
    addForm.style.display = "none";

//open and close filter form
    if (filterForm.style.display == "none" || filterForm.style.display == "") {
        filterForm.style.display = "block";
    } else {
        filterForm.style.display = "none";
    }
}

//will hide or show the new article form 
function showAddNew() {
    var filterForm = document.getElementById("filterContent");
    var addForm    = document.getElementById("newContent");
    filterForm.style.display = "none";

//add form toggle
    if (addForm.style.display == "none" || addForm.style.display == "") {
        addForm.style.display = "flex";
    } else {
        addForm.style.display = "none";
    }
}

//filter articles based on checkboxes
function filterArticles() {
    var showOpinions = document.getElementById("opinionCheckbox").checked;
    var showRecipes  = document.getElementById("recipeCheckbox").checked;
    var showUpdates  = document.getElementById("updateCheckbox").checked;

//for opinion articles
    var opinionArticles = document.getElementsByClassName("opinion");
    for (var i = 0; i < opinionArticles.length; i++) {
        if (showOpinions) {
            opinionArticles[i].style.display = "";
        } else {
            opinionArticles[i].style.display = "none";
        }
    }

//for recipe articles
    var recipeArticles = document.getElementsByClassName("recipe");
    for (var i = 0; i < recipeArticles.length; i++) {
        if (showRecipes) {
            recipeArticles[i].style.display = "";
        } else {
            recipeArticles[i].style.display = "none";
        }
    }

//for update articles
    var updateArticles = document.getElementsByClassName("update");
    for (var i = 0; i < updateArticles.length; i++) {
        if (showUpdates) {
            updateArticles[i].style.display = "";
        } else {
            updateArticles[i].style.display = "none";
        }
    }
}

//to add a new article
function addNewArticle() {
    var title = document.getElementById("inputHeader").value;
    var text  = document.getElementById("inputArticle").value;

    var typeClass = "";
    var typeLabel = "";

    if (document.getElementById("opinionRadio").checked) {
        typeClass = "opinion";
        typeLabel = "Opinion";
    } else if (document.getElementById("recipeRadio").checked) {
        typeClass = "recipe";
        typeLabel = "Recipe";
    } else if (document.getElementById("lifeRadio").checked) {
        typeClass = "update";
        typeLabel = "Update";
    }

//this makes sure the fields are entered
    if (title == "") {
        alert("Please enter a title.");
        return;
    }
    if (text == "") {
        alert("Please enter article text.");
        return;
    }
    if (typeClass == "") {
        alert("Please select an article type.");
        return;
    }

//makes new article
    var newArticle = document.createElement("article");
    newArticle.className = typeClass;

//adds the marker span
    var marker = document.createElement("span");
    marker.className = "marker";
    marker.textContent = typeLabel;
    newArticle.appendChild(marker);

//adds the title
    var heading = document.createElement("h2");
    heading.textContent = title;
    newArticle.appendChild(heading);

//adds the text
    var paragraph = document.createElement("p");
    paragraph.textContent = text;
    newArticle.appendChild(paragraph);

//adds the article to the page
    document.getElementById("articleList").appendChild(newArticle);

//filter so the article meets the requirements
    filterArticles();

//clear all forms after…
    document.getElementById("inputHeader").value    = "";
    document.getElementById("inputArticle").value   = "";
    document.getElementById("opinionRadio").checked = false;
    document.getElementById("recipeRadio").checked  = false;
    document.getElementById("lifeRadio").checked    = false;
}