$(document).ready(function () {
    $.ajaxSetup({cache: false});

    let locale = window.navigator.userLanguage || window.navigator.language;
    moment.locale(locale);

    refreshPage();

    setInterval(refreshPage, 5000);

    function refreshPage() {
        $('#table').html("");

        $.getJSON('data.json', data => {
            $.each(data['songs'].reverse(), (index, data) => {
                if (data != null) {
                    let content = `<tr><td>${data.title}</td><td>${data.artist}</td><td>${moment.unix(data.time).format('LLL')}</td><td>${moment.unix(data.time).fromNow()}</td><td><a href="https://www.youtube.com/results?search_query=${data.title} ${data.artist}">YouTube</a></td></tr>`;
                    $('#table').append(content);
                }
            });
        });
    }
});