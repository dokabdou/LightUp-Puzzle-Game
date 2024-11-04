/************************* INITIALIZATION *************************/

var canvas = document.getElementById('gamecanvas');
var g;

var clickX;
var clickY;
var i;
var j;

var S_BLANK = 0;        /**< a blank square */
var S_LIGHTBULB = 1;    /**< a light bulb */
var S_MARK = 2;         /**< a marked square (as not-a-light) */
var S_BLACK = 8;        /**< black wall base code */
var S_BLACK0 = S_BLACK; /**< a numbered black wall (with 0 adjacent lights) */
var S_BLACK1 = 9;           /**< a numbered black wall (with 1 adjacent light) */
var S_BLACK2 = 10;           /**< a numbered black wall (with 2 adjacent lights) */
var S_BLACK3 = 11;           /**< a numbered black wall (with 3 adjacent lights) */
var S_BLACK4 = 12;           /**< a numbered black wall (with 4 adjacent lights) */
var S_BLACKU = 13; /**< an unnumbered black wall (any number of adjacent lights) */
/* flags */
var F_LIGHTED = 16; /**< lighted flag */
var F_ERROR = 32; /**< error flag */

var babysun = new Image();
babysun.src = "png/Babysun.png";
var star = new Image();
star.src = "png/star.png";
var mark = new Image();
mark.src = "png/mark_satellite.png";
var asteroid_0 = new Image();
asteroid_0.src = "png/asteroid_0.png";
var asteroid_1 = new Image();
asteroid_1.src = "png/asteroid_1.png";
var asteroid_2 = new Image();
asteroid_2.src = "png/asteroid_2.png";
var asteroid_3 = new Image();
asteroid_3.src = "png/asteroid_3.png";
var asteroid_4 = new Image();
asteroid_4.src = "png/asteroid_4.png";
var asteroid_u = new Image();
asteroid_u.src = "png/asteroid_u.png";

var music = new Audio('sounds/astraljourney.mp3');
music.volume = 0.5;
var babysound = new Audio('sounds/babysound.mp3');
var winsound = new Audio('sounds/winsound.mp3');
winsound.volume = 0.5;
var sunsound = new Audio('sounds/sunsound.mp3');
sunsound.volume = 0.35;
var marksound = new Audio('sounds/marksound.mp3');
var restartsound = new Audio('sounds/restartsound.mp3');
var undosound = new Audio('sounds/undosound.mp3');
var redosound = new Audio('sounds/redosound.mp3');
var newgamesound = new Audio('sounds/newgamesound.mp3');
var errorsound = new Audio('sounds/errorsound.mp3');
errorsound.volume = 0.35;
var helpsound = new Audio('sounds/helpsound.mp3');
var removesunsound = new Audio('sounds/removesunsound.mp3');
var removemarksound = new Audio('sounds/removemarksound.mp3');

var nblasterror = 0;
var nbcurrenterror = 0;
var newerror = false;

window.addEventListener('load', windowLoad);              // window load
canvas.addEventListener('click', canvasLeftClick);        // left click event
canvas.addEventListener('contextmenu', canvasRightClick); // right click event

function windowLoad() {
    console.log("window load");
    g = Module._new_default();
    canvas.height = Module._nb_rows(g) * 100;
    canvas.width = Module._nb_cols(g) * 100;
    win();
    drawCanvas();
}


/* *********************** MUSIC & SOUNDS *********************** */

function musicplay(){
    music.play();
}
setInterval(musicplay,1000);

function soundreset(){
    babysound.pause();
    babysound.currentTime = 0;
    winsound.pause();
    winsound.currentTime = 0;
    sunsound.pause();
    sunsound.currentTime = 0;
    marksound.pause();
    marksound.currentTime = 0;
    restartsound.pause();
    restartsound.currentTime = 0;
    undosound.pause();
    undosound.currentTime = 0;
    redosound.pause();
    redosound.currentTime = 0;
    newgamesound.pause();
    newgamesound.currentTime = 0;
    errorsound.pause();
    errorsound.currentTime = 0;
    helpsound.pause();
    helpsound.currentTime = 0;
    removesunsound.pause();
    removesunsound.currentTime = 0;
    removemarksound.pause();
    removemarksound.currentTime = 0;
}


/* *********************** PLAYING MOVES *********************** */

function canvasLeftClick(event) {
    event.preventDefault(); // prevent default context menu to appear...
    // get relative cursor position in canvas
    console.log("left click at position:", event.offsetX, event.offsetY);
    // update position of vipere image used by drawCanvas()
    clickX = event.offsetX;
    clickY = event.offsetY;
    j = Math.floor(clickX / 100);
    i = Math.floor(clickY / 100);
    if (Module._is_blank(g, i, j)) {
        Module._play_move(g, i, j, S_LIGHTBULB);
        newerror = newError();
        soundreset();
        if(newerror){
            errorsound.play();
        } else {
            sunsound.play();
        }
    } else if (Module._is_lightbulb(g, i, j)) {
        Module._play_move(g, i, j, S_BLANK);
        newerror = newError();
        soundreset();
        removesunsound.play();
    }
    win();
    drawCanvas();
}

function canvasRightClick(event) {
    event.preventDefault(); // prevent default context menu to appear...
    // get relative cursor position in canvas
    console.log("right click at position:", event.offsetX, event.offsetY);
    // update position of vipere image used by drawCanvas()
    clickX = event.offsetX;
    clickY = event.offsetY;
    j = Math.floor(clickX / 100);
    i = Math.floor(clickY / 100);
    if (Module._is_blank(g, i, j)) {
        Module._play_move(g, i, j, S_MARK);
        newerror = newError();
        soundreset();
        if(newerror){
            errorsound.play();
        } else {
            marksound.play();
        }
    } else if (Module._is_marked(g, i, j)) {
        Module._play_move(g, i, j, S_BLANK);
        newerror = newError();
        soundreset();
        removemarksound.play();
    }
    win();
    drawCanvas();
}


/* *********************** WIN & ERRORS *********************** */

function win() {
    if (Module._is_over(g)) {
        document.getElementById('gamecanvas').style.backgroundImage = "url(png/space_win.png)"
        document.getElementById('gamecanvas').style.backgroundPosition = "center";
        soundreset();
        sunsound.play();
        babysound.play();
        winsound.play();
    } else {
        document.getElementById('gamecanvas').style.backgroundImage = "url(png/space.png)";
        document.getElementById('gamecanvas').style.backgroundPosition = "0% 0%";
    }
}

function newError(){
    for (x = 0; x < Module._nb_rows(g); x++) {
        for (y = 0; y < Module._nb_cols(g); y++) {
            if(Module._has_error(g,x,y)){
                nbcurrenterror++;
            }
        }
    }
    if(nbcurrenterror > nblasterror){
        nblasterror = nbcurrenterror;
        nbcurrenterror = 0;
        return true;
    } else {
        nblasterror = nbcurrenterror;
        nbcurrenterror = 0;
        return false;
    }
}

/* *********************** BUTTONS *********************** */

function restart() {
    Module._restart(g);
    newerror = newError();
    soundreset();
    restartsound.play();
    win();
    drawCanvas();
}

function solve() {
    Module._solve(g);
    newerror = newError();
    win();
    drawCanvas();
}

function undo() {
    Module._undo(g);
    newerror = newError();
    soundreset();
    undosound.play();
    win();
    drawCanvas();
}

function redo() {
    Module._redo(g);
    newerror = newError();
    soundreset();
    redosound.play();
    win();
    drawCanvas();
}

function genrandomnumber(min, max) {
    return Math.floor(Math.random() * (max - min)) + min;
}

function random() {
    var selection = document.getElementById("wall-choice");
    var p_w = selection.options[selection.selectedIndex].value;

    const wrap = document.getElementById('wrappingB').checked;
    if (document.getElementById('squareG').checked == true) {
        var rs = genrandomnumber(3, 10); // random number betweem 4 and 10
        var nw = p_w / 100 * (rs * rs);
        //console.log(rs);
        canvas.height = rs * 100;
        canvas.width = rs * 100;
        console.log((rs * rs))
        console.log(nw);
        g = Module._new_random(rs, rs, wrap, nw, false);
        newerror = newError();
        soundreset();
        newgamesound.play();
    } else {
        var rows = genrandomnumber(3, 10); // random number betweem 4 and 10
        var cols = genrandomnumber(3, 10); // random number betweem 4 and 10
        canvas.height = rows * 100;
        canvas.width = cols * 100;
        var n_w = p_w / 100 * (rows * cols); // percentage of black squares (rowscols)
        g = Module._new_random(rows, cols, wrap, n_w, false);
        newerror = newError();
        soundreset();
        newgamesound.play();
    }
    win();
    drawCanvas();
}

/* *********************** RULES / HELP *********************** */

var interval = setInterval(helpanimation,700);;
var helpshowed = false;

function helpon() {
    var popup = document.getElementById("rulespopup");
    popup.classList.add("show");
    clearInterval(interval);
    if (helpshowed == false){
        helpshowed = true;
        soundreset();
        helpsound.play();
    }
}

function helpoff() {
    var popup = document.getElementById("rulespopup");
    popup.classList.remove("show");
    interval = setInterval(helpanimation,700);
}

function helpanimation(){
    let elm = document.getElementById("rules")
    if (elm.textContent == "> RULES <"){
        elm.textContent = " >RULES< ";
        helpshowed = false;
    } else if (elm.textContent == " >RULES< "){
        elm.textContent = "> RULES <";
        helpshowed = false;
    }
}





/* *********************** DRAWING CANVAS *********************** */

function drawCanvas() {
    var ctx = canvas.getContext('2d');
    var width = canvas.width;
    var height = canvas.height;

    for (x = 0; x < Module._nb_rows(g); x++) {
        for (y = 0; y < Module._nb_cols(g); y++) {

            if (Module._is_lightbulb(g, x, y) && !(Module._has_error(g, x, y))) {
                ctx.clearRect(y * 100, x * 100, 100, 100);
                ctx.drawImage(babysun, y * 100, x * 100, 100, 100);
            }

            else if (Module._is_lightbulb(g, x, y) && (Module._has_error(g, x, y))) {
                ctx.filter = 'invert(1)'
                ctx.clearRect(y * 100, x * 100, 100, 100);
                ctx.drawImage(babysun, y * 100, x * 100, 100, 100);
                ctx.filter = 'invert(0)'
            }

            else if (Module._is_lighted(g, x, y) && !(Module._is_lightbulb(g, x, y)) && !(Module._is_marked(g, x, y)) && !(Module._is_black(g, x, y))) {  // square is lighted
                ctx.clearRect(y * 100, x * 100, 100, 100);
                ctx.drawImage(star, y * 100, x * 100, 100, 100);
            }

            else if (Module._is_black(g, x, y) && !(Module._has_error(g, x, y))) {
                if (Module._get_black_number(g, x, y) == 0) {
                    ctx.clearRect(y * 100, x * 100, 100, 100);
                    ctx.drawImage(asteroid_0, y * 100, x * 100, 100, 100);
                }
                else if (Module._get_black_number(g, x, y) == 1) {
                    ctx.clearRect(y * 100, x * 100, 100, 100);
                    ctx.drawImage(asteroid_1, y * 100, x * 100, 100, 100);
                }
                else if (Module._get_black_number(g, x, y) == 2) {
                    ctx.clearRect(y * 100, x * 100, 100, 100);
                    ctx.drawImage(asteroid_2, y * 100, x * 100, 100, 100);
                }
                else if (Module._get_black_number(g, x, y) == 3) {
                    ctx.clearRect(y * 100, x * 100, 100, 100);
                    ctx.drawImage(asteroid_3, y * 100, x * 100, 100, 100);
                }
                else if (Module._get_black_number(g, x, y) == 4) {
                    ctx.clearRect(y * 100, x * 100, 100, 100);
                    ctx.drawImage(asteroid_4, y * 100, x * 100, 100, 100);
                } else {
                    ctx.clearRect(y * 100, x * 100, 100, 100);
                    ctx.drawImage(asteroid_u, y * 100, x * 100, 100, 100);
                }
            }

            else if (Module._is_black(g, x, y) && (Module._has_error(g, x, y))) {
                ctx.filter = 'invert(1) sepia(1)'
                if (Module._get_black_number(g, x, y) == 0) {
                    ctx.clearRect(y * 100, x * 100, 100, 100);
                    ctx.drawImage(asteroid_0, y * 100, x * 100, 100, 100);
                }
                else if (Module._get_black_number(g, x, y) == 1) {
                    ctx.clearRect(y * 100, x * 100, 100, 100);
                    ctx.drawImage(asteroid_1, y * 100, x * 100, 100, 100);
                }
                else if (Module._get_black_number(g, x, y) == 2) {
                    ctx.clearRect(y * 100, x * 100, 100, 100);
                    ctx.drawImage(asteroid_2, y * 100, x * 100, 100, 100);
                }
                else if (Module._get_black_number(g, x, y) == 3) {
                    ctx.clearRect(y * 100, x * 100, 100, 100);
                    ctx.drawImage(asteroid_3, y * 100, x * 100, 100, 100);
                }
                else if (Module._get_black_number(g, x, y) == 4) {
                    ctx.clearRect(y * 100, x * 100, 100, 100);
                    ctx.drawImage(asteroid_4, y * 100, x * 100, 100, 100);
                } else {
                    ctx.clearRect(y * 100, x * 100, 100, 100);
                    ctx.drawImage(asteroid_u, y * 100, x * 100, 100, 100);
                }
                ctx.filter = 'invert(0) sepia(0)'
            }

            else if (Module._is_marked(g, x, y)) {
                ctx.clearRect(y * 100, x * 100, 100, 100);
                ctx.drawImage(mark, y * 100, x * 100, 100, 100);
            }

            else {
                ctx.clearRect(y * 100, x * 100, 100, 100);
            }

        }
    }
}


