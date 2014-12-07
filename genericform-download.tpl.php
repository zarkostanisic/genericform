<?php
/**
 * @file
 *
 * Theme for show name of a file and download link
 */
?>
<p class="item-download">
  <span class="download-link">
    <a href="<?php if(isset($path)): echo $path; endif; ?>" target="_blank">
      <?php if(isset($name)): echo ucfirst($name); endif; ?>
    </a>
  </span>
</p>
