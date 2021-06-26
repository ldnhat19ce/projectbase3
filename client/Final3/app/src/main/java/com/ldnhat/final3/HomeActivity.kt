package com.ldnhat.final3

import android.Manifest
import android.annotation.SuppressLint
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Build
import android.os.Bundle
import android.speech.RecognitionListener
import android.speech.RecognizerIntent
import android.speech.SpeechRecognizer
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.DatabaseReference
import com.google.firebase.database.ValueEventListener
import com.google.firebase.database.ktx.database
import com.google.firebase.ktx.Firebase
import kotlinx.android.synthetic.main.activity_home.*
import java.util.*

class HomeActivity : AppCompatActivity() {

    private lateinit var speechRecognizer:SpeechRecognizer
    val RecordAudioRequestCode = 1
    var ref: DatabaseReference? = null
    var fan:DatabaseReference? = null
    var lightData:String? = null
    var fanData:String? = null

    @SuppressLint("ClickableViewAccessibility")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_home)

        if (ContextCompat.checkSelfPermission(
                this,
                Manifest.permission.RECORD_AUDIO
            ) != PackageManager.PERMISSION_GRANTED
        ) {
            checkPermission()
        }

        val database = Firebase.database
        ref = database.getReference("light_state")
        fan = database.getReference("fan_state")

        ref!!.addValueEventListener(object : ValueEventListener{
            override fun onCancelled(error: DatabaseError) {
                error.details
            }

            override fun onDataChange(snapshot: DataSnapshot) {
               lightData = snapshot.value as String
                handleStateOfLight(lightData!!)

            }

        })

        fan!!.addValueEventListener(object : ValueEventListener{
            override fun onCancelled(error: DatabaseError) {
                error.details
            }

            override fun onDataChange(snapshot: DataSnapshot) {
                fanData = snapshot.value as String
                handleStateOfFan(fanData!!)
            }

        })

        speechRecognizer =  SpeechRecognizer.createSpeechRecognizer(this)

        val intent = Intent(RecognizerIntent.ACTION_RECOGNIZE_SPEECH)
        intent.putExtra(RecognizerIntent.EXTRA_LANGUAGE_MODEL, RecognizerIntent.LANGUAGE_MODEL_FREE_FORM)
        intent.putExtra(RecognizerIntent.EXTRA_LANGUAGE, Locale.getDefault())

        btn_open_voice.setOnClickListener {
            startActivityForResult(intent, 20)
        }

    }


    private fun handleStateOfFan(data: String){
        sw_turn_fan.isChecked = data == "turn_on_fan"

        sw_turn_fan.setOnCheckedChangeListener { buttonView, isChecked ->
            if(isChecked){
                fan!!.setValue("turn_on_fan")
                Toast.makeText(this, "đã bật quạt", Toast.LENGTH_SHORT).show()
            }else{
                fan!!.setValue("turn_off_fan")
                Toast.makeText(this, "đã tắt quạt", Toast.LENGTH_SHORT).show()
            }
        }
    }

    private fun handleStateOfLight(data : String){
        sw_turn_light.isChecked = data == "turn_on_light"
        sw_turn_light.setOnCheckedChangeListener { buttonView, isChecked ->
            if(isChecked){
                ref!!.setValue("turn_on_light")
                Toast.makeText(this, "đã bật đèn", Toast.LENGTH_SHORT).show()
            }else if(!isChecked){
                ref!!.setValue("turn_off_light")
                Toast.makeText(this, "đã tắt đèn", Toast.LENGTH_SHORT).show()
            }
        }
    }

    private fun handleSpeechRecognizer(speechRecognizer: SpeechRecognizer){
        speechRecognizer.setRecognitionListener(object : RecognitionListener{
            override fun onReadyForSpeech(params: Bundle?) {

            }

            override fun onRmsChanged(rmsdB: Float) {

            }

            override fun onBufferReceived(buffer: ByteArray?) {

            }

            override fun onPartialResults(partialResults: Bundle?) {

            }

            override fun onEvent(eventType: Int, params: Bundle?) {

            }

            override fun onBeginningOfSpeech() {
                //txt_result.hint = "hello"
            }

            override fun onEndOfSpeech() {

            }

            override fun onError(error: Int) {

            }

            override fun onResults(results: Bundle?) {
                if (results != null) {
                    val bundle: ArrayList<String>? =  results.getStringArrayList(SpeechRecognizer.RESULTS_RECOGNITION)
                    val data = bundle?.get(0)
                    //txt_result.text = data.toString()
                }
            }

        })
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if(requestCode == 20 && data != null){
            val intent: ArrayList<String>? =  data.getStringArrayListExtra(RecognizerIntent.EXTRA_RESULTS)
            val voice = intent?.get(0)

            if(voice.equals("turn on light") || voice.equals("Turn on Light") || voice.equals("Bật Đèn")
                || voice.equals("bật đèn")){
                ref?.setValue("turn_on_light")
            }else if(voice.equals("Tắt Đèn") || voice.equals("Turn off Light")) {
                ref?.setValue("turn_off_light")
            }else if(voice.equals("Turn on Fan") || voice.equals("Bật Quạt") || voice.equals("bật quạt")){
                fan?.setValue("turn_on_fan")
            }else if(voice.equals("Turn off Fan") || voice.equals("Tắt Quạt") || voice.equals("tắt quạt")){
                fan?.setValue("turn_off_fan")
            }
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        speechRecognizer.destroy()
    }

    private fun checkPermission() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            ActivityCompat.requestPermissions(
                this,
                arrayOf(Manifest.permission.RECORD_AUDIO),
                this.RecordAudioRequestCode
            )
        }
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        if (requestCode == this.RecordAudioRequestCode && grantResults.size > 0) {
            if (grantResults[0] == PackageManager.PERMISSION_GRANTED) Toast.makeText(
                this,
                "Permission Granted",
                Toast.LENGTH_SHORT
            ).show()
        }
    }

}