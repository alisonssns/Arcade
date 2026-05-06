const slider_holder = document.getElementById('slider_holder')
const counter = document.getElementById('counter')
const nome_jogo = document.getElementById('nome_jogo')

const games = [
    { nome: "League of legends", img: "https://t2.tudocdn.net/537860?w=1920" },
    { nome: "Valorant", img: "https://www.riotgames.com/darkroom/1200/1dbd7211e78ce5faa7a8af9d10afad47:2b5979e3922758399ba389561e797919/ps-f2p-val-console-launch-16x9.jpg" },
    { nome: "Street Fighter", img: "https://www.nintendo.com/eu/media/images/10_share_images/games_15/super_nintendo_5/H2x1_SNES_StreetFighterIITurboHyperFighting_image1600w.jpg" },
    { nome: "Mario world", img: "https://www.nintendo.com/eu/media/images/10_share_images/games_15/super_nintendo_5/H2x1_SNES_SuperMarioWorld_image1600w.jpg" },
    { nome: "Counter Strike 1.6", img: "https://shared.akamai.steamstatic.com/store_item_assets/steam/apps/10/header.jpg?t=1745368572" },
]

const games_qtd = games.length
const meio = Math.floor(games_qtd / 2)

for (i in games) {
    let slide = document.createElement('div')
    let slide_image = document.createElement('img')

    slide.className = "slide"
    slide.style.perspective = "1000px"

    slide_image.src = games[i].img
    slide_image.className = "background"

    slide.appendChild(slide_image)

    slider_holder.appendChild(slide)
}

const slides = document.querySelectorAll(".slide")
const slide_size = slides[0].offsetWidth
const adjust = games_qtd % 2 == 0 ? slide_size / 2 : 0
slider_holder.style.width = `${100 * games_qtd}%`

const min_slide = -meio
const max_slide = games_qtd - meio - 1

selected = 0

function changeSlide(num) {
    selected += num
    const selected_index = selected + meio
    for (let i = 0; i < games_qtd; ++i) {
        const pos = i - selected_index

        if (i == selected_index) {
            slider_holder.style.transform = `translateX(${(slide_size * -selected) - adjust}px)`
            slides[i].style.filter = "none"
            if (i < games_qtd - 1) slides[i + 1].style.filter = `brightness(0.5)`
            if (i > 0) slides[i - 1].style.filter = `brightness(0.5)`
            slides[i].style.zIndex = "10"
        } else {
            slides[i].style.zIndex = "0"
        }
        slides[i].style.transform = `translate(${-(pos) * slide_size / 2}px) rotateY(${-(pos) * 35}deg) scale(${1 - (0.5 * (Math.abs(pos)))})`
    }
    counter.textContent = `${selected_index + 1}/${games_qtd}`
    nome_jogo.textContent = `${games[selected_index].nome}`
}

window.addEventListener("keydown", (event) => {
    tecla = event.key.toLowerCase()
    if ((tecla == "arrowleft" || tecla == "a") && selected > min_slide) {
        changeSlide(-1)
    } else if ((tecla == "arrowright" || tecla == "d") && selected < max_slide) {
        changeSlide(+1)
    }
})

changeSlide(0)