//Drupal.behaviors.contact_map = function (context) {
function ($) {
$(document).ready( function() {
    $('#plus a').click(function(e) {
        $('#myMap').attr('style', cm.maxSize);
        $('#plus').addClass('hide');
        $('#minus').removeClass('hide');
        e.preventDefault();
    })
    $('#minus a').click(function(e) {
        $('#myMap').attr('style', cm.minSize);
        $('#minus').addClass('hide');
        $('#plus').removeClass('hide');
        e.preventDefault();
    })
})
}(jQuery)
//}
