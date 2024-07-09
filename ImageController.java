package org.example;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.core.io.FileSystemResource;
import org.springframework.core.io.Resource;
import org.springframework.http.HttpHeaders;
import org.springframework.http.MediaType;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import java.io.File;
import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

@RestController
public class ImageController {
    @Autowired
    private ImageService imageService;

    @CrossOrigin(origins = "http://192.168.19.1:5173")
    @GetMapping("/check-downloaded-images")
    public Map<String, Object> checkDownloadedImages() {
        List<Map<String, String>> imageInfoList = imageService.getImageInfoList();
        Map<String, Object> response = new HashMap<>();
        List<Map<String, String>> validImageInfoList = imageInfoList.stream()
                .filter(imageInfo -> new File(imageInfo.get("path")).exists())
                .map(imageInfo -> {
                    try {
                        imageInfo.put("url", "http://localhost:8080/image?path=" + URLEncoder.encode(imageInfo.get("path"), StandardCharsets.UTF_8.toString()));
                    } catch (UnsupportedEncodingException e) {
                        e.printStackTrace();
                    }
                    return imageInfo;
                })
                .collect(Collectors.toList());

        if (!validImageInfoList.isEmpty()) {
            response.put("imagesAvailable", true);
            response.put("imageInfoList", validImageInfoList);
        } else {
            response.put("imagesAvailable", false);
        }
        return response;
    }

    @GetMapping("/image")
    public ResponseEntity<Resource> getImage(@RequestParam String path) {
        Resource resource = new FileSystemResource(path);
        if (!resource.exists()) {
            return ResponseEntity.notFound().build();
        }

        HttpHeaders headers = new HttpHeaders();
        headers.add(HttpHeaders.CONTENT_TYPE, MediaType.IMAGE_JPEG_VALUE);
        return ResponseEntity.ok().headers(headers).body(resource);
    }
}
