// Esperamos a que todo el contenido del HTML esté cargado antes de ejecutar el script
document.addEventListener("DOMContentLoaded", () => {
    
    // Obtenemos referencias a los elementos del HTML que necesitamos
    const form = document.getElementById("player-lookup-form");
    const gameNameInput = document.getElementById("gameName");
    const tagLineInput = document.getElementById("tagLine");
    const regionSelect = document.getElementById("region");
    const resultsDiv = document.getElementById("results");

    // Añadimos un "escuchador" al evento 'submit' del formulario
    form.addEventListener("submit", async (event) => {
        // Prevenimos que el formulario se envíe de la forma tradicional (recargando la página)
        event.preventDefault();

        // 1. Obtenemos los valores de los inputs
        const gameName = gameNameInput.value;
        const tagLine = tagLineInput.value;
        const region = regionSelect.value;

        // 2. Limpiamos los resultados anteriores y mostramos un mensaje de "cargando"
        resultsDiv.textContent = "Buscando jugador...";

        // 3. ¡AQUÍ VIENE LA MAGIA!
        // Esta es la llamada (fetch) a nuestro propio backend en C++
        // Nota: La URL 'http://localhost:18080' es un EJEMPLO.
        // Debe coincidir con el puerto que tu servidor C++ (Crow) esté escuchando.
        
        // Codificamos los componentes para que se puedan usar en una URL
        const encodedGameName = encodeURIComponent(gameName);
        const encodedTagLine = encodeURIComponent(tagLine);

        // Construimos la URL a la que llamará el frontend
        const apiUrl = `http://localhost:18080/api/player/${encodedGameName}/${encodedTagLine}?region=${region}`;

        try {
            // 4. Realizamos la petición a nuestro backend
            const response = await fetch(apiUrl);

            if (!response.ok) {
                // Si el backend C++ devuelve un error (ej: 404, 500)
                throw new Error(`Error del servidor: ${response.status} ${response.statusText}`);
            }

            // 5. Convertimos la respuesta (que será JSON) a un objeto JavaScript
            const data = await response.json();

            // 6. Mostramos los datos en la página de forma bonita
            // JSON.stringify(data, null, 2) formatea el JSON para que sea legible
            resultsDiv.textContent = JSON.stringify(data, null, 2);

        } catch (error) {
            // Si hay un error de red (ej: el servidor C++ no está encendido)
            resultsDiv.textContent = `Error al conectar con el backend: ${error.message}`;
            console.error("Error en el fetch:", error);
        }
    });
});