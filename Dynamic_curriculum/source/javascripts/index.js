const scrollContent = document.querySelector('.scroll-content');
    const scrollThumb = document.querySelector('.custom-scrollbar-thumb');
    const scrollTrack = document.querySelector('.custom-scrollbar-track');

    function updateThumbPosition() {
      const contentHeight = scrollContent.scrollHeight;
      const visibleHeight = scrollContent.clientHeight;
      const scrollRatio = visibleHeight / contentHeight;
      const thumbHeight = scrollTrack.clientHeight * scrollRatio;
      const scrollTop = scrollContent.scrollTop;
      const thumbPosition = (scrollTop / (contentHeight - visibleHeight)) * (scrollTrack.clientHeight - thumbHeight);

      scrollThumb.style.height = `${thumbHeight}px`;
      scrollThumb.style.transform = `translateY(${thumbPosition}px)`;
    }

    scrollContent.addEventListener('scroll', updateThumbPosition);

    // Initial update
    updateThumbPosition();

    // Optional: Implement drag functionality for the thumb
    let isDragging = false;
    let startY;
    let startScrollTop;

    scrollThumb.addEventListener('mousedown', (e) => {
      isDragging = true;
      startY = e.clientY;
      startScrollTop = scrollContent.scrollTop;
      scrollThumb.style.cursor = 'grabbing';
    });

    document.addEventListener('mousemove', (e) => {
      if (!isDragging) return;
      const deltaY = e.clientY - startY;
      const contentHeight = scrollContent.scrollHeight;
      const visibleHeight = scrollContent.clientHeight;
      const scrollableHeight = contentHeight - visibleHeight;
      const trackHeight = scrollTrack.clientHeight;
      const thumbHeight = scrollThumb.clientHeight;
      const scrollRatio = scrollableHeight / (trackHeight - thumbHeight);
      const newScrollTop = startScrollTop + (deltaY * scrollRatio);

      scrollContent.scrollTop = newScrollTop;
    });

    document.addEventListener('mouseup', () => {
      isDragging = false;
      scrollThumb.style.cursor = 'grab';
    });

    // This space is intentionally left blank. The drag and drop functionality has been moved to drag_drop.js