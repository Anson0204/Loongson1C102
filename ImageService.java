package org.example;

import org.springframework.stereotype.Service;
import java.io.File;
import java.io.IOException;
import java.nio.file.*;
import java.text.SimpleDateFormat;
import java.util.*;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.stream.Collectors;

@Service
public class ImageService {
    private static final String DOWNLOADS_DIRECTORY = "C:/Users/16131/Downloads";
    private static final String SAVE_DIRECTORY = "E:/save";
    private static final String IMAGE_EXTENSION = ".jpg";
    private final List<Map<String, String>> imageInfoList = new ArrayList<>();
    private final AtomicInteger imageCounter = new AtomicInteger(1);

    public ImageService() {
        watchDirectory();
        scanSavedDirectory();
    }

    private void watchDirectory() {
        try {
            WatchService watchService = FileSystems.getDefault().newWatchService();
            Path path = Paths.get(DOWNLOADS_DIRECTORY);
            path.register(watchService, StandardWatchEventKinds.ENTRY_CREATE);

            Thread thread = new Thread(() -> {
                while (true) {
                    try {
                        WatchKey key = watchService.take();
                        for (WatchEvent<?> event : key.pollEvents()) {
                            WatchEvent.Kind<?> kind = event.kind();
                            if (kind == StandardWatchEventKinds.ENTRY_CREATE) {
                                Path filePath = (Path) event.context();
                                if (filePath.toString().endsWith(IMAGE_EXTENSION)) {
                                    String sourcePath = DOWNLOADS_DIRECTORY + "/" + filePath.toString();
                                    String targetPath = SAVE_DIRECTORY + "/imga(" + imageCounter.getAndIncrement() + ")" + IMAGE_EXTENSION;
                                    moveAndRenameFile(sourcePath, targetPath);
                                }
                            }
                        }
                        key.reset();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            });
            thread.setDaemon(true);
            thread.start();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void moveAndRenameFile(String sourcePath, String targetPath) throws IOException {
        Files.move(Paths.get(sourcePath), Paths.get(targetPath), StandardCopyOption.REPLACE_EXISTING);
        String downloadTime = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date());
        Map<String, String> imageInfo = new HashMap<>();
        imageInfo.put("path", targetPath);
        imageInfo.put("downloadTime", downloadTime);
        imageInfoList.add(imageInfo);
    }

    private void scanSavedDirectory() {
        File folder = new File(SAVE_DIRECTORY);
        File[] listOfFiles = folder.listFiles((dir, name) -> name.toLowerCase().endsWith(IMAGE_EXTENSION));
        Arrays.stream(listOfFiles).forEach(file -> {
            String downloadTime = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date(file.lastModified()));
            Map<String, String> imageInfo = new HashMap<>();
            imageInfo.put("path", file.getAbsolutePath());
            imageInfo.put("downloadTime", downloadTime);
            imageInfoList.add(imageInfo);
        });
        imageCounter.set(listOfFiles.length + 1);
    }

    public List<Map<String, String>> getImageInfoList() {
        return imageInfoList.stream()
                .sorted(Comparator.comparing(info -> info.get("downloadTime")))
                .collect(Collectors.toList());
    }
}
